#include "raylib.h"
#include "fluidbox.h"
#include <cstddef>
#include <vector>
#include <iostream>

#define TESTS

int main(void) {
  const int screenWidth = 15;
  const int screenHeight = 15;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  
  #ifdef TESTS
  FluidBox box = FluidBox(screenWidth, screenHeight, 0.5);
  //box.testAddDensitySource();
  box.testAddVelocitySource();
  //box.testDiffuseDensity();
  box.testDiffuseVelocity();
  //box.printVector(box.getDensityField());
  box.printVector(box.getVelocityField());
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
