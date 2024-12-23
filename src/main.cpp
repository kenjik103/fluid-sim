#include "fluidbox.h"
#include "raylib.h"
#include <cstddef>
#include <vector>

int main(void) {
  const int screenWidth{1280};
  const int screenHeight{720};

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  // fluidbox configurations:
  const int nodeSize{8}; // PLEASE make sure this is a nice number
  const float timestep{0.01f};
  const float diffusionRate{0.005f};
  const float viscosity{5.f};

  FluidBox fluidBox = FluidBox(screenWidth / nodeSize, screenHeight / nodeSize,
                               timestep, viscosity, diffusionRate);
  Vector2 mousePosition{};

  // Main game loop
  while (!WindowShouldClose()) // etect window close button or ESC key
  {

    mousePosition = GetMousePosition();
    mousePosition.x /= nodeSize;
    mousePosition.y /= nodeSize;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      fluidBox.add_density(static_cast<int>(mousePosition.x),
                           static_cast<int>(mousePosition.y), 50.f);
    }
    fluidBox.update_velocity();
    fluidBox.update_density();
    // fluidBox.printVector(fluidBox.getDensityField());

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    for (int y{}; y < screenHeight / nodeSize; y++) {
      for (int x{}; x < screenWidth / nodeSize; x++) {
        float density = fluidBox.getDensityNodeValue(x, y);
        if (density != 0) {
          DrawRectangle(x * nodeSize, y * nodeSize, nodeSize, nodeSize, BLACK);
        }
      }
    }
    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  // De-Initialization
  CloseWindow();

  return 0;
}
