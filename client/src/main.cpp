#include "boundaryRecognition.h"

int main(int argc, char** argv) {
  cv::String key = "{imagePath | none | input image}";
  cv::CommandLineParser parser(argc, argv, key);

  std::string imagePath = parser.get<std::string>("imagePath");

  if (imagePath != "none") {
    BoundaryRecognition singleRecognition(imagePath);
    Mat singleImageBoundaries = singleRecognition.getBoundaries();
  } 

  return 0;
}