#include "boundaryRecognition.h"

int main() {
  httplib::Server srvr;
  srvr.listen("0.0.0.0", 8080);

  srvr.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
    std::vector<char> buffer(req.body.begin(), req.body.end());
    Mat input = cv::imdecode(Mat(buffer), cv::IMREAD_GRAYSCALE);

    BoundaryRecognition br(input);
    Mat output = br.getBoundaries();

    vector<uchar> buf;
    cv::imencode(".jpg", output, buf);

    std::string encodedImage(buf.begin(), buf.end());

    res.set_content(encodedImage, "image.jpg");
  });

  
  return 0;
}