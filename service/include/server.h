#include "boundaryRecognition.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void http_server(tcp::acceptor& acceptor, tcp::socket& socket);
void log(const std::string& message);

namespace my_progran_state {
  std::size_t request_count();
  std::time_t now();
}

class http_connection : public std::enable_shared_from_this<http_connection> {
public:
  http_connection(tcp::socket socket);

  void start();

private:
  tcp::socket socket_;

  beast::flat_buffer buffer_{8192};

  http::request<http::dynamic_body> request_;

  http::response<http::dynamic_body> response_;

  net::steady_timer deadline_{
    socket_.get_executor(), std::chrono::seconds(60)};

  void read_request(); // Asynchronously receive a complete request message.
  void process_request(); // Determine what needs to be done with the request message.
  void create_response(); // Construct a response message based on the program state.
  void write_response(); // Asynchronously transmit the response message.
  void check_deadline(); // Check whether we have spent enough time on this connection.
}; 