#include <array>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "../include/platforms.hpp"


namespace {

using namespace cv;

}  // namespace


Platforms::Platforms(int screen_width, int screen_height)
    : screen_width_(screen_width), screen_height_(screen_height) {
  std::random_device rd;
  gen_ = std::mt19937(rd());
}

void Platforms::updateRandomPlatforms(int platforms_count) {
  std::uniform_real_distribution<> x_distrib(0, screen_width_);
  std::uniform_real_distribution<> y_distrib(0, screen_height_);

  platforms_.clear();
  platforms_.reserve(platforms_count);

  for (int i = 0; i < platforms_count; ++i) {
    const int y = static_cast<int>(y_distrib(gen_));
    const int x = static_cast<int>(x_distrib(gen_));

    platforms_.push_back({y, x, x + PLATFORM_LENGTH});
  }
}


void Platforms::updatePlatformsFromScreenshot(const std::string& img_path) {
  Mat img = imread(img_path, IMREAD_GRAYSCALE);
  if (img.empty()) {
    std::cerr << "Could not read the image: " << img_path << '\n';
    return;
  }

  Mat img_with_edges;
  Canny(img, img_with_edges, 0, 100);

  std::vector<Vec4i> lines;
  HoughLinesP(img_with_edges, lines, 1, CV_PI / 90, 100, 200, 0);

  platforms_.clear();
  for (const Vec4i& line_coords : lines) {
    int x0 = line_coords[0];
    int y0 = line_coords[1];
    int x1 = line_coords[2];
    int y1 = line_coords[3];

    if (std::abs(y0 - y1) <= EPSILON) {
      platforms_.push_back({(y0 + y1) / 2, x0, x1});
    }
  }
}
