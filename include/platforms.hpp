#ifndef PLATFORMS_HPP
#define PLATFORMS_HPP

#include <array>
#include <iostream>
#include <random>
#include <vector>


class Platforms {
 public:
  const static int PLATFORM_LENGTH = 200;
  const static int EPSILON         = 5;

  Platforms() {}
  Platforms(int screen_width, int screen_height);

  void updateRandomPlatforms(int platforms_count);

  void updatePlatformsFromScreenshot(const std::string& img_path);

  const std::vector<std::array<int, 3>>& getPlatforms() const {
    return platforms_;
  }

 private:
  int screen_width_  = 0;
  int screen_height_ = 0;

  std::vector<std::array<int, 3>> platforms_;  // {y, x_start, x_end}

  std::mt19937 gen_;
};

#endif  // PLATFORMS_HPP
