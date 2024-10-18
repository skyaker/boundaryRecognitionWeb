#include "server.h"

void http_server(tcp::acceptor& acceptor, tcp::socket& socket) {
  acceptor.async_accept(socket, [&](beast::error_code ec) {
    if (!ec)
      std::make_shared<http_connection>(std::move(socket))->start();
    
    http_server(acceptor, socket);
  });
}

void log(const std::string& message) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::cout<<"["<<std::ctime(&now_time)<<"]"<<message<<std::endl;
}

std::size_t my_progran_state::request_count() {
  static std::size_t count = 0;
  return ++count;
}

std::time_t my_progran_state::now() {
  return std::time(0);
}

http_connection::http_connection(tcp::socket socket) : socket_(std::move(socket)) {}

void http_connection::start() {
  log("Start processing");
  read_request();
  check_deadline();
}

void http_connection::read_request() {
  auto self = shared_from_this();
  http::async_read(
    socket_, 
    buffer_, 
    request_,
    [self](beast::error_code ec, std::size_t bytes_transferred) {
      boost::ignore_unused(bytes_transferred);
      if(!ec)
        self->process_request();
    }
  );
}

// void http_connection::read_request() {
//     auto self = shared_from_this();
//     http::async_read(
//         socket_,
//         buffer_,
//         request_,
//         [self](beast::error_code ec, std::size_t bytes_transferred) {
//             boost::ignore_unused(bytes_transferred);

//             if (!ec) {
//                 // 1. Определение метода запроса
//                 auto method = self->request_.method();
//                 std::cout << "Method: " << self->request_.method_string() << std::endl;

//                 // 2. Определение размера тела запроса
//                 std::size_t body_size = self->request_.body().size();
//                 std::cout << "Body size: " << body_size << " bytes" << std::endl;

//                 // 3. Определение целевого URL
//                 std::cout << "Target: " << self->request_.target() << std::endl;

//                 // Вызываем обработку запроса
//                 self->process_request();
//             } else {
//                 std::cerr << "Error reading request: " << ec.message() << std::endl;
//             }
//         }
//     );
// }

void http_connection::process_request() {
  response_.version(request_.version());
  response_.keep_alive(false);

  if (request_.method() == http::verb::post && request_.target() == "/process") {
    log("Received POST request on /process");

    auto& body = request_.body(); // Получаем тело запроса (multi_buffer)
    std::vector<char> buffer;     // Буфер для данных

    // Итерируем через все сегменты multi_buffer и копируем их в буфер
    for (auto const& segment : body.data()) {
      buffer.insert(buffer.end(),
                    boost::asio::buffers_begin(segment),
                    boost::asio::buffers_end(segment));
    }
    
    if (buffer.empty()) {
      log("Buffer is empty");
      response_.result(http::status::bad_request);
      response_.set(http::field::content_type, "text/plain");
      beast::ostream(response_.body()) << "Request body is empty";
      return write_response();
    }

    cv::Mat input = cv::imdecode(cv::Mat(buffer), cv::IMREAD_GRAYSCALE);

    if (input.empty()) {
      log("Failed to decode image");
      response_.result(http::status::bad_request);
      response_.set(http::field::content_type, "text/plain");
      beast::ostream(response_.body())<<"Failed to decode image";
      return write_response();
    }

    BoundaryRecognition br(input);
    cv::Mat output = br.getBoundaries();

    std::vector<uchar> buf;

    if (!cv::imencode(".jpg", output, buf)) {
      log("Falied to encode image");
      response_.result(http::status::internal_server_error);
      response_.set(http::field::content_type, "text/plain");
      beast::ostream(response_.body())<<"Failed to encode image.";
      return write_response();
    }

    log("Successfully processed image");

    response_.result(http::status::ok);
    response_.set(http::field::content_type, "image/jpeg");
    beast::ostream(response_.body())<<std::string(buf.begin(), buf.end());
  }
  else if (request_.method() == http::verb::get && request_.target() == "/") {
    log("Received GET request on /");
    response_.result(http::status::ok);
    response_.set(http::field::content_type, "text/plain");
    beast::ostream(response_.body())<<"GET request received";
  }
  else {
    response_.result(http::status::not_found);
    response_.set(http::field::content_type, "text/plain");
    beast::ostream(response_.body())<<"Not found";
  }

  write_response();
}

void http_connection::write_response() {
  auto self = shared_from_this();
  response_.content_length(response_.body().size());
  http::async_write(
    socket_, 
    response_,
    [self](beast::error_code ec, std::size_t) {
      self->socket_.shutdown(tcp::socket::shutdown_send, ec);
      self->deadline_.cancel();
    }
  );
}

void http_connection::check_deadline() {
  auto self = shared_from_this();

  deadline_.async_wait(
    [self](beast::error_code ec) {
      if (!ec) 
        self->socket_.close(ec);
    }
  );
}