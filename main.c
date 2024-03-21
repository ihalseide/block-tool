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


int cubeGrass, cubeDirt, cubeGrass2, cubeStone;


void initCubeDrawingAtlas(Texture2D texture) {
    CubeDrawingAtlas *cda = &state->cubeDrawAtlas;
    *cda = makeCubeDrawingAtlas16(texture);

    int squareGrassSide = cubeDrawingAtlasAddSquareFromIndex(cda, 3);
    int squareGrass = cubeDrawingAtlasAddSquareFromIndex(cda, 4);
    int squareDirt = cubeDrawingAtlasAddSquareFromIndex(cda, 2);
    int squareStone = cubeDrawingAtlasAddSquareFromIndex(cda, 1);

    cubeGrass = cubeDrawingAtlasAddCube(cda, makeBlockDrawingKind3(squareGrassSide, squareGrass, squareDirt));
    cubeDirt = cubeDrawingAtlasAddCube(cda, makeBlockDrawingKind1(squareDirt));
    cubeGrass2 = cubeDrawingAtlasAddCube(cda, makeBlockDrawingKind1(squareGrass));
    cubeStone = cubeDrawingAtlasAddCube(cda, makeBlockDrawingKind1(squareStone));
}


int main() {
    // Zero-initialize the whole game state
    state = (FaceCraft*) malloc(sizeof(*state));
    memset(state, 0, sizeof(*state));

    float cubeSize = 1;

    float lookXSpeed = 0.1f;
    float lookYSpeed = 0.1f;

    // Initialize window and drawing
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "FaceCraft");
    SetTargetFPS(60);

    initCubeDrawingAtlas(LoadTexture("texture.png"));

    // Init camera
    state->cam.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    state->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    state->cam.target = (Vector3){ 0.0f, 2.0f, 4.0f };
    state->cam.fovy = 70.0f;
    state->cam.projection = CAMERA_PERSPECTIVE;

    // Place some blocks
    blocksSetBlockAt(&state->daBlocks, makeBlockPosition(1,1,1), cubeGrass);
    blocksSetBlockAt(&state->daBlocks, makeBlockPosition(1,5,1), cubeGrass);
    blocksSetBlockAt(&state->daBlocks, makeBlockPosition(2,1,2), cubeDirt);
    blocksSetBlockAt(&state->daBlocks, makeBlockPosition(3,1,2), cubeGrass2);
    for (int x = -10; x < -2; x++) {
        blocksSetBlockAt(&state->daBlocks, makeBlockPosition(x,1,2), cubeStone);
    }

    // Grab cursor for 3D looking around
    DisableCursor();

    // Main game loop
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            if (cubeSize < 1) {
                cubeSize = 1;
            }
            else {
                cubeSize = 0.8f;
            }
        }

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
                    drawBlockKindAt(&state->cubeDrawAtlas, bp.blockDrawingKindIndex, centerPos, cubeSize, WHITE, -1);
                }
            }
            EndMode3D();

            // Draw center crosshairs
            Color crossColor = WHITE;
            int crossSize = 10;
            int centerX = GetScreenWidth()/2;
            int centerY = GetScreenHeight()/2;
            DrawLine(centerX-crossSize, centerY, centerX+crossSize, centerY, crossColor);
            DrawLine(centerX, centerY-crossSize, centerX, centerY+crossSize, crossColor);

            DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)",
                state->cam.position.x, state->cam.position.y, state->cam.position.z),
                610, 60, 10, BLACK);
        }
        EndDrawing();
    }
    
    CloseWindow();
}