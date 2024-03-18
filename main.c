#include <stdio.h>
#include "raylib/raylib.h"
#include "types.h"


void globalCleanup() {
    CloseWindow();
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "FaceCraft");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello! --FaceCraft", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    globalCleanup();
}