#include <stdio.h>
#include "raylib/raylib.h"
#include "types.h"
#include "stb_ds.h"


static FaceCraft state;


void globalCleanup() {
    CloseWindow();
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "FaceCraft");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Zero-initialize the whole game state
    memset(&state, 0, sizeof(state));

    // Init camera
    state.camera.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    state.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    state.camera.target = (Vector3){ 0.0f, 2.0f, 4.0f };
    state.camera.fovy = 70.0f;
    state.camera.projection = CAMERA_PERSPECTIVE;

    // Prepare to enter game loop
    DisableCursor();

    float lookXSpeed = 0.1f;
    float lookYSpeed = 0.1f;

    // Main game loop
    while (!WindowShouldClose()) {

        // Do camera movement update
        UpdateCameraPro(&state.camera,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,    
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                0.0f                                                // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*lookXSpeed,                            // Rotation: yaw
                GetMouseDelta().y*lookYSpeed,                            // Rotation: pitch
                0.0f                                                // Rotation: roll
            },
            0.0f); 

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            BeginMode3D(state.camera);
            {
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLUE);
                DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);
            }
            EndMode3D();
            DrawText("Hello! --FaceCraft", 190, 200, 20, LIGHTGRAY);
        }
        EndDrawing();
    }
    globalCleanup();
}