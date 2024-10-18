#include "boundaryRecognition.h"
#include "server.h"

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << "\n";
      return EXIT_FAILURE;
    }

    auto const address = net::ip::make_address(argv[1]);
    unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

    net::io_context ioc{1};
    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};

    http_server(acceptor, socket);
    ioc.run();
  }
  catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}

// void log(const std::string& message) {
//   auto now = std::chrono::system_clock::now();
//   std::time_t now_time = std::chrono::system_clock::to_time_t(now);
//   std::cout << "[" << std::ctime(&now_time) << "] " << message << std::endl;
// }

// int main() {
//   httplib::Server srvr;

//   srvr.Get("/", [](const httplib::Request &req, httplib::Response &res) {
//     std::cout << "Received GET request on /" << std::endl;
//     res.set_content("GET request received", "text/plain");
//   });

//   srvr.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
//     std::vector<char> buffer(req.body.begin(), req.body.end());

//     Mat input = cv::imdecode(Mat(buffer), cv::IMREAD_GRAYSCALE);

//     BoundaryRecognition br(input);
//     Mat output = br.getBoundaries();

//     vector<uchar> buf;
//     cv::imencode(".jpg", output, buf);

//     std::string encodedImage(buf.begin(), buf.end());

//     res.set_content(encodedImage, "image.jpg");
//   });

//   srvr.listen("0.0.0.0", 8080);
  
//   return 0;
// }