#include <array>
#include <random>
#include <utility>
#include <vector>


namespace {

std::vector<std::array<float, 3>> curr_platforms;

std::random_device rd;
std::mt19937 gen(rd());

const float platform_len = 200.0f;

}  // namespace


void initRandomPlatforms(const int SCREEN_WIDTH, const int SCREEN_HEIGHT,
                         const int platforms_count) {
  std::uniform_real_distribution<float> x_distrib(0, SCREEN_WIDTH);
  std::uniform_real_distribution<float> y_distrib(0, SCREEN_HEIGHT);

  curr_platforms.reserve(platforms_count);

  for (int i = 0; i < platforms_count; ++i) {
    const float y = y_distrib(gen);
    const float x = x_distrib(gen);

    curr_platforms.push_back({y, x, x + platform_len});
  }
}


// TODO: make sure that the actual logic for getting platforms only happens if
// the screen has been updated, otherwise .
const std::vector<std::array<float, 3>> getPlatforms(
  const int SCREEN_WIDTH, const int SCREEN_HEIGHT
) {
  return curr_platforms;
}
