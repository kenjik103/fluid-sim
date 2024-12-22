#include "fluidbox.h"
#include "raylib.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <vector>


int main(void) {
  const int screenWidth = 30;
  const int screenHeight = 30;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  FluidBox fluidBox = FluidBox(screenWidth, screenHeight, 0.001f);
  Vector2 mousePosition{};

  // Main game loop
  while (!WindowShouldClose()) // etect window close button or ESC key
  {

    mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
      fluidBox.add_density(mousePosition, 1.f);
    }

    fluidBox.update_density(0.005f);
    fluidBox.printVector(fluidBox.getDensityField());

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int y{}; y < screenHeight; ++y){
      for (int x{}; x < screenWidth; ++x){
        float density = fluidBox.getDensityNodeValue(x,y);
        density = std::clamp(density, 0.f, 255.f);
        std::uint8_t balls = (std::uint8_t) std::lerp(0.f, 1.f, density);
        Color color = Color(balls, balls, balls);
        DrawPixel(x, y, color);
      }
    }

    EndDrawing();
  }

  // De-Initialization
  CloseWindow();

  return 0;
}
