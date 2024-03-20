#include <stdio.h>

// raylib
#include "raylib/raylib.h"

// Data structure types
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

// FaceCraft types
#include "types.h"
#include "drawing.h"
#include "blocks.h"


// Main global game state (not directly accessible to the other source code files)
static FaceCraft *state;


int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    float lookXSpeed = 0.1f;
    float lookYSpeed = 0.1f;

    InitWindow(screenWidth, screenHeight, "FaceCraft");
    SetTargetFPS(60);

    // Zero-initialize the whole game state
    state = (FaceCraft*) malloc(sizeof(*state));
    memset(state, 0, sizeof(*state));

    // Init camera
    state->cam.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    state->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    state->cam.target = (Vector3){ 0.0f, 2.0f, 4.0f };
    state->cam.fovy = 70.0f;
    state->cam.projection = CAMERA_PERSPECTIVE;

    // Init blocks
    mapAddBlockKind(&state->map, makeBlockKind(0));
    mapSetBlock(&state->map, makeBlockPosition(1, 1, 1), 0);
    printf("There are %ld block kinds\n", arrlen(state->map.blockKinds.da_start));
    printf("There are %ld blocks\n", arrlen(state->map.blocks.da_start));

    Image img = LoadImage("texture.png");
    ImageFlipVertical(&img);
    Texture2D tex = LoadTextureFromImage(img);
    

    // Prepare to enter game loop
    DisableCursor();

    // Main game loop
    while (!WindowShouldClose()) {

        // Do camera movement update
        UpdateCameraPro(&state->cam,
            (Vector3){
                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f - // Move forward-backward
                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,
                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f - // Move right-left
                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                0.0f // Move up-down
            },
            (Vector3){
                GetMouseDelta().x*lookXSpeed, // Rotation: yaw
                GetMouseDelta().y*lookYSpeed, // Rotation: pitch
                0.0f // Rotation: roll
            },
            0.0f); 

        BeginDrawing();
        {
            ClearBackground(SKYBLUE);
            BeginMode3D(state->cam);
            {
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLUE);
                DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);
                for (int i = 0; i < arrlen(state->map.blocks.da_start); ++i) {
                    BlockPair b = state->map.blocks.da_start[i];
                    //DrawCube(mapBlockPositionToVector3(b.pos), 1, 1, 1, c);
                    drawCubeTexture(tex, mapBlockPositionToVector3(b.pos), 0.95f, 0.95f, 0.95f, WHITE);
                    //drawCubeFaceTexture(tex, mapBlockPositionToVector3(b.pos), 1, WHITE, i % (CUBE_DIRECTION_BACK+1));
                }
            }
            EndMode3D();
            DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)",
                state->cam.position.x, state->cam.position.y, state->cam.position.z),
                610, 60, 10, GREEN);
        }
        EndDrawing();
    }
    
    CloseWindow();
}