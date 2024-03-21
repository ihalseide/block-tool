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
#include "cubeAtlas.h"


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

    Image img = LoadImage("texture.png");
    //ImageFlipVertical(&img);
    Texture2D tex = LoadTextureFromImage(img);

    // Init blocks
    state->cubeDrawAtlas = makeCubeDrawingAtlas(tex, 2, 2);
    int grassBottom = cubeDrawingAtlasAddSquare(&state->cubeDrawAtlas, 0, 1);
    int grassSide = cubeDrawingAtlasAddSquare(&state->cubeDrawAtlas, 1, 0);
    int grassTop = cubeDrawingAtlasAddSquare(&state->cubeDrawAtlas, 1, 1);
    BlockDrawingKind grassBlock = makeBlockKind(grassSide);
    grassBlock.faces.topIndex = grassTop;
    grassBlock.faces.bottomIndex = grassBottom;
    int grassId = cubeDrawingAtlasAddCube(&state->cubeDrawAtlas, grassBlock);

    blocksSetBlockAt(&state->daBlocks, makeBlockPosition(1,1,1), grassId);

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
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BROWN);
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIGHTGRAY);
                for (int i = 0; i < arrlen(state->daBlocks); ++i) {
                    BlockPair bp = state->daBlocks[i];
                    Vector3 centerPos = mapBlockPositionToVector3(bp.pos);
                    float size = 0.95f;
                    drawBlockKindAt(&state->cubeDrawAtlas, bp.kindIndex, centerPos, size, WHITE, -1);
                }
            }
            EndMode3D();
            DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)",
                state->cam.position.x, state->cam.position.y, state->cam.position.z),
                610, 60, 10, BLACK);
        }
        EndDrawing();
    }
    
    CloseWindow();
}