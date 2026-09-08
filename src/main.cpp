#include <limits>
#include <optional>
#include <string>
#include <unordered_set>

#include "raylib.h"

#include "../include/platforms.hpp"
#include "../include/utilities.hpp"


namespace {

// Constants and globals.
const std::string GAME_TITLE             = "Screen Jumper";
const int         RANDOM_PLATFORMS_COUNT = 20;
const char*       EXAMPLE_1_IMAGE_PATH   = "../assets/example1.png";
const char*       EXAMPLE_2_IMAGE_PATH   = "../assets/example2.png";
int               SCREEN_WIDTH;
int               SCREEN_HEIGHT;

struct GameObject {
  Vector2 dim = {100.0f, 100.0f};  // Defaults to 100x100.
  Vector2 pos;
};
GameObject lil_guy;
GameObject hand;
GameObject arm;
Platforms platforms;


// Movement consts.
constexpr float Y_ACCELERATION = 0.9f;
constexpr float WALK_SPEED     = 7.5f;
constexpr float JUMP_SPEED     = 20.0f;

// Movement variables.
float y_velocity = 0.0f;

// Flags.
bool can_jump   = false;
bool hand_is_on = false;


/// HELPERS.

bool positionIsInBounds(Vector2 new_pos) {
  const float x = new_pos.x;
  const float y = new_pos.y;
  return 0 <= x && x + lil_guy.dim.x <= GetScreenWidth() &&
         y + lil_guy.dim.y <= GetScreenHeight();
}


std::optional<float> getYPosIfLanded(
  const int SCREEN_WIDTH,
  const int SCREEN_HEIGHT,
  Vector2 new_pos
) {
  const float old_player_bottom = lil_guy.pos.y + lil_guy.dim.y;
  const float player_bottom     = new_pos.y + lil_guy.dim.y;
  const float player_left       = new_pos.x;
  const float player_right      = new_pos.x + lil_guy.dim.x;

  float landed_y_pos  = std::numeric_limits<float>::max();

  // Find the highest platform the player now overlaps with.
  for (auto [y, x0, x1] : platforms.getPlatforms()) {
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


void startFallingIfInAir(
  const int SCREEN_WIDTH,
  const int SCREEN_HEIGHT,
  Vector2 new_pos
) {
  if (new_pos.y + lil_guy.dim.y < SCREEN_HEIGHT &&
      !getYPosIfLanded(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos).has_value()) {
    can_jump = false;
  }
}


bool thisLandedOnThat(
  const GameObject& obj1,
  const GameObject& obj2,
  Vector2 new_pos
) {
  // Return false if obj1 was not above obj2 in the first place.
  if (obj1.pos.y + obj1.dim.y > obj2.pos.y) {
    return false;
  }

  float obj1_bottom = new_pos.y + obj1.dim.y;
  float obj1_left   = new_pos.x;
  float obj1_right  = obj1_left + obj1.dim.x;

  float obj2_top   = obj2.pos.y;
  float obj2_left  = obj2.pos.x;
  float obj2_right = obj2_left + obj2.dim.x;

  return (obj1_bottom >= obj2_top && obj1_left < obj2_right &&
          obj1_right > obj2_left);
}


void toggleHand() {
  hand_is_on = !hand_is_on;
  if (IsCursorHidden()) {
    ShowCursor();
  } else {
    HideCursor();
  }
}


void updateHand() {
  if (hand_is_on) {
    hand.pos = {
      GetMouseX() - (hand.dim.x / 2),
      GetMouseY() - (hand.dim.y / 2)
    };
    arm.pos = {
      hand.pos.x - arm.dim.x,
      (hand.dim.y - arm.dim.y) / 2 + hand.pos.y
    };
  }
}


void updateLilGuy() {
  if (IsKeyDown(KEY_RIGHT)) {
    const Vector2 new_pos = {lil_guy.pos.x + WALK_SPEED, lil_guy.pos.y};
    if (positionIsInBounds(new_pos)) { lil_guy.pos.x = new_pos.x; }
    startFallingIfInAir(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos);
  }

  if (IsKeyDown(KEY_LEFT)) {
    const Vector2 new_pos = {lil_guy.pos.x - WALK_SPEED, lil_guy.pos.y};
    if (positionIsInBounds(new_pos)) { lil_guy.pos.x = new_pos.x; }
    startFallingIfInAir(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos);
  }

  if (can_jump) {  // i.e., player is on ground/platform.
    if (IsKeyDown(KEY_UP)) {
      can_jump   = false;
      y_velocity = -JUMP_SPEED;
    }

    if (lil_guy.pos.y + lil_guy.dim.y < SCREEN_HEIGHT && IsKeyDown(KEY_DOWN)) {
      can_jump = false;
      lil_guy.pos.y += 1;
    }
  } else {  // i.e., player is in the air.
    const Vector2 new_pos = {lil_guy.pos.x, lil_guy.pos.y + y_velocity};

    if (positionIsInBounds(new_pos)) {
      // Stop falling if landed on platform.
      const std::optional<float> landed_platform_y_pos =
          getYPosIfLanded(SCREEN_WIDTH, SCREEN_HEIGHT, new_pos);

      if (y_velocity >= 0 && landed_platform_y_pos) {
        lil_guy.pos.y = landed_platform_y_pos.value() - lil_guy.dim.y;
        y_velocity    = 0.0f;
        can_jump      = true;
      } else if (hand_is_on && y_velocity >= 0 &&
                 thisLandedOnThat(lil_guy, hand, new_pos)) {
        lil_guy.pos.y = hand.pos.y - lil_guy.dim.y;
        y_velocity    = 0.0f;
        can_jump      = true;
      } else {
        lil_guy.pos.y = new_pos.y;
        y_velocity    += Y_ACCELERATION;
      }
    } else {
      lil_guy.pos.y = SCREEN_HEIGHT - lil_guy.dim.y;
      y_velocity    = 0.0f;
      can_jump      = true;
    }
  }
}


void drawPlatforms() {
  for (auto [y, x0, x1] : platforms.getPlatforms()) {
    DrawLine(
      static_cast<int>(x0),
      static_cast<int>(y),
      static_cast<int>(x1),
      static_cast<int>(y),
      VIOLET
    );
  }
}

}  // namespace


int main() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED |
                 /*FLAG_WINDOW_TOPMOST | FLAG_WINDOW_MOUSE_PASSTHROUGH |*/
                 FLAG_FULLSCREEN_MODE);
  InitWindow(0, 0, "Screen Jumper");

  SCREEN_WIDTH  = GetScreenWidth();
  SCREEN_HEIGHT = GetScreenHeight();

  // Textures.
  Texture2D BACKGROUND = LoadTexture(EXAMPLE_1_IMAGE_PATH);

  // Game objects/interactables.
  lil_guy.dim = {50.0f, 50.0f};
  lil_guy.pos = {
    (static_cast<float>(SCREEN_WIDTH)  + lil_guy.dim.x) / 2.0f,
    (static_cast<float>(SCREEN_HEIGHT) - lil_guy.dim.y)
  };
  hand.dim = {120.0f, 24.0f};
  arm.dim  = {static_cast<float>(SCREEN_WIDTH), 10.0f};
  // When on, hand/arm positions will follow mouse.
  platforms = Platforms(SCREEN_WIDTH, SCREEN_HEIGHT);

  SetTargetFPS(60);
  platforms.updatePlatformsFromScreenshot(EXAMPLE_2_IMAGE_PATH);

  while (!WindowShouldClose()) {
    /// UPDATE.

    if (IsKeyPressed(KEY_GRAVE)) {
      toggleHand();
    }

    updateLilGuy();
    updateHand();


    /// DRAW.

    BeginDrawing();
      ClearBackground(BLANK);

      DrawTexture(BACKGROUND, 0, 0, WHITE);
      drawPlatforms();
      if (hand_is_on) {
        DrawRectangleV(hand.pos, hand.dim, RAYWHITE);
        DrawRectangleV(arm.pos, arm.dim, BLACK);
      }
      DrawRectangleV(lil_guy.pos, lil_guy.dim, MAGENTA);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
