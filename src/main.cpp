#include "raylib.h"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    DrawFPS(GetMouseX(), GetMouseY());

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("wawawwaawlkdjflkajdf", 190, 200, 20,
             LIGHTGRAY);

    EndDrawing();
  }

  // De-Initialization
  CloseWindow(); 

  return 0;
}
