#include "fluidbox.h"
#include "raylib.h"
#include <cstddef>
#include <iostream>
#include <vector>

#define TESTS

int main(void) {
  const int screenWidth = 15;
  const int screenHeight = 15;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

#ifdef TESTS
  FluidBox box = FluidBox(screenWidth, screenHeight, 1);
  box.testAddDensitySource();
  box.testAddVelocitySource();
  // box.testDiffuseDensity();
  // box.testDiffuseVelocity();
  box.update_density(0.005f);
  std::cout << "PREV DENSITY:" << "\n";
  box.printVector(box.getPrevDensityField());
  std::cout << "CURRENT DENSITY:" << "\n";
  box.printVector(box.getDensityField());
#endif

  // Main game loop
  while (!WindowShouldClose()) // etect window close button or ESC key
  {
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  // De-Initialization
  CloseWindow();

  return 0;
}
