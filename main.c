#include <stdio.h>
#include "raylib/raylib.h"
#include "types.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"


static FaceCraft *state;


void globalCleanup() {
    CloseWindow();
}

// Convert a `BlockPosition` to a raylib `Vector3`.
Vector3 mapBlockPositionToVector3(BlockPosition blockPos) {
    return (Vector3){ blockPos.x, blockPos.y, blockPos.z };
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "FaceCraft");
    SetTargetFPS(60);

    // Zero-initialize the whole game state
    state = (FaceCraft*) malloc(sizeof(*state));
    memset(state, 0, sizeof(state));

    // Init camera
    state->camera.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    state->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    state->camera.target = (Vector3){ 0.0f, 2.0f, 4.0f };
    state->camera.fovy = 70.0f;
    state->camera.projection = CAMERA_PERSPECTIVE;

    // Init block
    state->blocks.da_start = NULL;
    Block new_block;
    new_block.kind = (BlockKind) { 1 };
    for (int i = 0; i < 10; i++) {
        new_block.pos = (BlockPosition){ i, 3, 3 };
        arrput(state->blocks.da_start, new_block);
    }

    // Prepare to enter game loop
    DisableCursor();

    float lookXSpeed = 0.1f;
    float lookYSpeed = 0.1f;

    // Main game loop
    while (!WindowShouldClose()) {

        // Do camera movement update
        UpdateCameraPro(&state->camera,
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
            BeginMode3D(state->camera);
            {
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLUE);
                DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);
                for (int i = 0; i < arrlen(state->blocks.da_start); ++i) {
                    Block b = state->blocks.da_start[i];
                    DrawCube(mapBlockPositionToVector3(b.pos), 1, 1, 1, RED);
                }
            }
            EndMode3D();
            DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)",
                state->camera.position.x, state->camera.position.y, state->camera.position.z),
                610, 60, 10, BLACK);
        }
        EndDrawing();
    }
    globalCleanup();
}