#include "boundaryRecognition.h"

BoundaryRecognition::BoundaryRecognition(const Mat& originalImage) : originalImage(originalImage) {}


void BoundaryRecognition::addAdaptiveBlur() {
  int kernelSize = 9;

  const int sigma = 1;

  Mat kernel = cv::getGaussianKernel(kernelSize, 1.0);
  Mat full_kernel = kernel * kernel.t();

  cv::filter2D(processingImage, processingImage, -1, full_kernel);
}

Mat BoundaryRecognition::findGradients() {
  Mat grad_x; 
  Mat grad_y; 
  Mat abs_grad_x; 
  Mat abs_grad_y;
  Mat grad_xf;
  Mat grad_yf;
  Mat directions;

  Sobel(processingImage, grad_x, CV_16S, 1, 0, 3);
  Sobel(processingImage, grad_y, CV_16S, 0, 1, 3);

  convertScaleAbs(grad_x, abs_grad_x);
  convertScaleAbs(grad_y, abs_grad_y);
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, processingImage);

  grad_x.convertTo(grad_xf, CV_32F);
  grad_y.convertTo(grad_yf, CV_32F);

  phase(grad_xf, grad_yf, directions, true);

  return directions;
}

void BoundaryRecognition::suppressionOfNonMaximums(Mat& directions) {
  Mat suppressed = Mat::zeros(processingImage.size(), processingImage.type());

  for (int i = 1; i < processingImage.rows - 1; ++i) {
    for (int j = 1; j < processingImage.cols - 1; ++j) {
      float angle;
      float current;
      float q;
      float r;

      angle = directions.at<float>(i, j);
      current = processingImage.at<uchar>(i, j);
      
      if ((angle >= -22.5 && angle < 22.5) || (angle >= 157.5 || angle < -157.5)) {
        q = processingImage.at<uchar>(i, j + 1);
        r = processingImage.at<uchar>(i, j - 1);
      } else if ((angle >= 22.5 && angle < 67.5) || (angle >= -157.5 && angle < -112.5)) {
        q = processingImage.at<uchar>(i + 1, j - 1);
        r = processingImage.at<uchar>(i - 1, j + 1);
      } else if ((angle >= 67.5 && angle < 112.5) || (angle >= -112.5 && angle < -67.5)) {
        q = processingImage.at<uchar>(i + 1, j);
        r = processingImage.at<uchar>(i - 1, j);
      } else if ((angle >= 112.5 && angle < 157.5) || (angle >= -67.5 && angle < -22.5)) {
        q = processingImage.at<uchar>(i - 1, j - 1);
        r = processingImage.at<uchar>(i + 1, j + 1);
      }

      if (current >= q && current >= r) {
        suppressed.at<uchar>(i, j) = current;
      }
    }
  }
  processingImage = suppressed.clone();
}

int BoundaryRecognition::getOtsuThresh() {
  Mat hist;
  int histSize;

  histSize = 256;
  const float range[] = {0, 256};
  const float* histRange = {range};

  calcHist(&processingImage, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

  if (hist.empty()) {
    std::cout << "Histogram is empty!" << std::endl;
    return -1;
  }

  double total = processingImage.total();
  float sumBackIntens = 0, sumBackPixels = 0, sumForePixels = 0, maxValue = 0;
  int threshold = 0;

  for (int t = 0; t < histSize; ++t) {
    sumBackPixels += hist.at<float>(t);
    if (sumBackPixels == 0) { continue; }

    sumForePixels = total - sumBackPixels;
    if (sumForePixels == 0) { break; }

    sumBackIntens += t * hist.at<float>(t);
    float meanBackIntens = sumBackIntens / sumBackPixels;
    float meanForeIntens = (total * t - sumBackIntens) / sumForePixels;

    float varBetween = sumBackPixels * sumForePixels * (meanBackIntens - meanForeIntens) * (meanBackIntens - meanForeIntens);

    if (varBetween > maxValue) {
      maxValue = varBetween;
      threshold = t;
    }
  }
  return threshold;
}

void BoundaryRecognition::applyDoubleThreshold() {
  int otsuValue = getOtsuThresh();

  int highThreshold = static_cast<int>(0.7 * otsuValue);
  int lowThreshold = static_cast<int>(0.3 * highThreshold);

  for (int i = 0; i < processingImage.rows; ++i) {
    for (int j = 0; j < processingImage.cols; ++j) {
      int pixel = processingImage.at<uchar>(i, j);
      if (pixel >= highThreshold) {
        processingImage.at<uchar>(i, j) = 255;
      } else if (pixel < lowThreshold) {
        processingImage.at<uchar>(i, j) = 0;
      } else {
        processingImage.at<uchar>(i, j) = 128;
      }
    }
  }
  
  Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
  morphologyEx(processingImage, processingImage, cv::MORPH_CLOSE, kernel);

  for (int i = 0; i < processingImage.rows; ++i) {
    for (int j = 0; j < processingImage.cols; ++j) {
      if (processingImage.at<uchar>(i, j) == 128) {
        processingImage.at<uchar>(i, j) = 255;
      }
    }
  }
}

cv::Mat BoundaryRecognition::getBoundaries() {
  // if (originalImage.depth() != CV_32F) {
  //   originalImage.convertTo(originalImage, CV_32F);
  // }

  cv::normalize(originalImage, originalImage, 0, 255, cv::NORM_MINMAX);
  originalImage.convertTo(originalImage, CV_8U);

  processingImage = originalImage.clone();

  addAdaptiveBlur();

  Mat directions = findGradients();

  suppressionOfNonMaximums(directions);

  applyDoubleThreshold();

  return processingImage;
}