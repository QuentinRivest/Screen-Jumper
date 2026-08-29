#include "raylib.h"

namespace {

// Lil guy specs.
const Vector2 LIL_GUY_DIM{25, 25};
Vector2 lil_guy_pos;

// Movement variables.
const float Y_ACCELERATION = 1.0f;
const float WALK_SPEED = 7.5f;
const float JUMP_SPEED = 20.0f;
float y_velocity = 0.0f;

// Flags.
bool is_on_ground = false;

}  // namespace


bool positionIsInBounds(Vector2 new_pos);
bool isOnPlatform(Vector2 new_pos);
void updateLilGuy(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);


int main(void) {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(0, 0, "Screen Jumper");
  SetWindowState(FLAG_WINDOW_UNDECORATED);

  const int SCREEN_WIDTH  = GetScreenWidth();
  const int SCREEN_HEIGHT = GetScreenHeight();

  lil_guy_pos = {
    (static_cast<float>(SCREEN_WIDTH)  + LIL_GUY_DIM.x) / 2.0f,
    (static_cast<float>(SCREEN_HEIGHT) + LIL_GUY_DIM.y) / 2.0f
  };

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    updateLilGuy(SCREEN_WIDTH, SCREEN_HEIGHT);

    BeginDrawing();
      ClearBackground(BLANK);
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


bool isOnPlatform(Vector2 new_pos) {
  // TODO: Get platforms. Logic for getting platforms should be function from
  // separate file since that'll link to the edge detection stuff.

  return false;
}


void updateLilGuy(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
  if (!is_on_ground) {
    const Vector2 new_pos = {lil_guy_pos.x, lil_guy_pos.y + y_velocity};

    if (positionIsInBounds(new_pos)) {
      lil_guy_pos.y = new_pos.y;
      y_velocity    += Y_ACCELERATION;

      if (isOnPlatform(new_pos)) {
        y_velocity    = 0.0f;
        is_on_ground  = true;
      }
    } else {
      lil_guy_pos.y = SCREEN_HEIGHT - LIL_GUY_DIM.y;
      y_velocity    = 0.0f;
      is_on_ground  = true;
    }
  } else if (IsKeyDown(KEY_UP)) {
    is_on_ground = false;
    y_velocity   = -JUMP_SPEED;
  }

  if (IsKeyDown(KEY_LEFT))  {
    const Vector2 new_pos = {lil_guy_pos.x - WALK_SPEED, lil_guy_pos.y};
    if (positionIsInBounds(new_pos)) { lil_guy_pos.x = new_pos.x; }
    // TODO: Handle falling once platforms exist.
  }

  if (IsKeyDown(KEY_RIGHT)) {
    const Vector2 new_pos = {lil_guy_pos.x + WALK_SPEED, lil_guy_pos.y};
    if (positionIsInBounds(new_pos)) { lil_guy_pos.x = new_pos.x; }
    // TODO: Handle falling once platforms exist.
  }
}
