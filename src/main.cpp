#include "fluidBox.h"
#include "raylib.h"

#include <cstddef>
#include <iostream>

int main(void) {
  const int fluidBoxSize{64};
  const int scale{10};

  const int screenWidth{fluidBoxSize * scale};
  const int screenHeight{fluidBoxSize * scale};

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  // fluidbox configurations:
  float dyeAmount{100.f};
  float diffusion{0.01f};
  float viscosity{0.05f};
  float timestep{0.001f};
  float fadeSpeed{.02f};
  float dyeBrightness{10.f};
  float velMultiplier{10.f};

  FluidBox fluidBox =
      FluidBox(fluidBoxSize, scale, diffusion, viscosity, timestep);
  int mouseX{};
  int mouseY{};
  Vector2 mouseDelta{};

  // Main game loop
  while (!WindowShouldClose()) // etect window close button or ESC key
  {

    mouseX = GetMouseX();
    mouseY = GetMouseY();
    mouseDelta = GetMouseDelta();
    mouseX = (mouseX > screenWidth) ? screenWidth : mouseX;
    mouseX = (mouseX < 0) ? 0 : mouseX;
    mouseY = (mouseY > screenHeight) ? screenHeight : mouseY;
    mouseY = (mouseY < 0) ? 0 : mouseY;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      fluidBox.addDensity(mouseX, mouseY, dyeAmount);
      fluidBox.addVelocity(mouseX, mouseY, mouseDelta.x * velMultiplier,
                           mouseDelta.y * velMultiplier);
    }

    fluidBox.step();

    // Draw
    BeginDrawing();
    fluidBox.render(fadeSpeed, dyeBrightness);
    ClearBackground(BLACK);

    EndDrawing();
  }

  // De-Initialization
  CloseWindow();

  return 0;
}
