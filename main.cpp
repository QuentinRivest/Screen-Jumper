#include <limits>
#include <optional>

#include "raylib.h"

#include "platforms.hpp"


namespace {

// Lil guy specs.
const Vector2 LIL_GUY_DIM{25, 25};
Vector2 lil_guy_pos;

// Movement variables.
const float Y_ACCELERATION = 0.9f;
const float WALK_SPEED     = 7.5f;
const float JUMP_SPEED     = 20.0f;
float       y_velocity     = 0.0f;

// Flags.
bool can_jump = false;

}  // namespace


/// HELPERS.
bool positionIsInBounds(Vector2 new_pos);
std::optional<float> getLandedYPos(const int SCREEN_WIDTH,
                                   const int SCREEN_HEIGHT,
                                   Vector2 new_pos);
void startFallingIfInAir(const int SCREEN_WIDTH,
                         const int SCREEN_HEIGHT,
                         Vector2 new_pos);
void updateLilGuy(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
void drawPlatforms(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);


int main(void) {
  // SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(0, 0, "Screen Jumper");
  SetWindowState(FLAG_WINDOW_UNDECORATED);

  const int SCREEN_WIDTH  = GetScreenWidth();
  const int SCREEN_HEIGHT = GetScreenHeight();

  lil_guy_pos = {
    (static_cast<float>(SCREEN_WIDTH)  + LIL_GUY_DIM.x) / 2.0f,
    (static_cast<float>(SCREEN_HEIGHT) - LIL_GUY_DIM.y)
  };

  SetTargetFPS(60);
  initRandomPlatforms(SCREEN_WIDTH, SCREEN_HEIGHT, 10);

  while (!WindowShouldClose()) {
    updateLilGuy(SCREEN_WIDTH, SCREEN_HEIGHT);

    BeginDrawing();
      ClearBackground(BLANK);

      drawPlatforms(SCREEN_WIDTH, SCREEN_HEIGHT);

      DrawRectangleV(lil_guy_pos, LIL_GUY_DIM, MAGENTA);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}


bool positionIsInBounds(Vector2 new_pos) {
  const float x = new_pos.x;
  const float y = new_pos.y;
  return 0 <= x && x + LIL_GUY_DIM.x <= GetScreenWidth() &&
         y + LIL_GUY_DIM.y <= GetScreenHeight();
}


std::optional<float> getLandedYPos(const int SCREEN_WIDTH,
                                   const int SCREEN_HEIGHT,
                                   Vector2 new_pos) {
  const float old_player_bottom = lil_guy_pos.y + LIL_GUY_DIM.y;
  const float player_bottom     = new_pos.y + LIL_GUY_DIM.y;
  const float player_left       = new_pos.x;
  const float player_right      = new_pos.x + LIL_GUY_DIM.x;

  float landed_y_pos  = std::numeric_limits<float>::max();

  // Find the highest platform the player now overlaps with.
  for (auto [y, x0, x1] : getPlatforms(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    if (old_player_bottom <= y && y <= player_bottom &&
        x0 <= player_right && player_left <= x1 &&
        y < landed_y_pos) {
      landed_y_pos = y;
    }
  }

  return landed_y_pos == std::numeric_limits<float>::max()
      ? std::nullopt
      : std::optional(landed_y_pos);
}


void startFallingIfInAir(const int SCREEN_WIDTH,
                         const int SCREEN_HEIGHT,
                         Vector2 new_pos) {
  if (new_pos.y + LIL_GUY_DIM.y < SCREEN_HEIGHT &&
      !getLandedYPos(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos).has_value()) {
    can_jump = false;
  }
}


void updateLilGuy(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
  if (IsKeyDown(KEY_LEFT))  {
    const Vector2 new_pos = {lil_guy_pos.x - WALK_SPEED, lil_guy_pos.y};
    if (positionIsInBounds(new_pos)) { lil_guy_pos.x = new_pos.x; }
    startFallingIfInAir(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos);
  }

  if (IsKeyDown(KEY_RIGHT)) {
    const Vector2 new_pos = {lil_guy_pos.x + WALK_SPEED, lil_guy_pos.y};
    if (positionIsInBounds(new_pos)) { lil_guy_pos.x = new_pos.x; }
    startFallingIfInAir(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos);
  }

  if (can_jump && IsKeyDown(KEY_UP)) {
    can_jump   = false;
    y_velocity = -JUMP_SPEED;
  }

  if (!can_jump) {
    const Vector2 new_pos = {lil_guy_pos.x, lil_guy_pos.y + y_velocity};

    if (positionIsInBounds(new_pos)) {
      // Stop falling if landed on platform.
      const std::optional<float> landed_y_pos =
      getLandedYPos(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos);
      if (y_velocity >= 0 && landed_y_pos.has_value()) {
        lil_guy_pos.y = landed_y_pos.value() - LIL_GUY_DIM.y;
        y_velocity    = 0.0f;
        can_jump      = true;
      } else {
        lil_guy_pos.y = new_pos.y;
        y_velocity    += Y_ACCELERATION;
      }
    } else {
      lil_guy_pos.y = SCREEN_HEIGHT - LIL_GUY_DIM.y;
      y_velocity    = 0.0f;
      can_jump      = true;
    }
  }
}


void drawPlatforms(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
  for (auto [y, x0, x1] : getPlatforms(SCREEN_WIDTH, SCREEN_HEIGHT)) {
    DrawLine(static_cast<int>(x0), static_cast<int>(y),
              static_cast<int>(x1), static_cast<int>(y),
              VIOLET);
  }
}
