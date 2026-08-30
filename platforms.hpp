#include <array>
#include <utility>
#include <vector>


void updateRandomPlatforms(const int SCREEN_WIDTH, const int SCREEN_HEIGHT,
                             const int platforms_count);

void updatePlatforms(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

std::vector<std::array<float, 3>> getPlatforms(const int SCREEN_WIDTH,
                                               const int SCREEN_HEIGHT);
