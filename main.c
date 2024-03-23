#include <stdio.h>

// raylib
#include "raylib/raylib.h"
#include "raylib/raymath.h"

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


// Initialize the drawing data for each cube/block kind.
void initCubeDrawingAtlas(Texture2D texture) {
    CubeDrawingAtlas *cda = &state->cubeDrawAtlas;
    *cda = makeCubeDrawingAtlas16(texture);

    int squareGrassSide = cubeDrawingAtlasAddSquareFromIndex(cda, 3);
    int squareGrass = cubeDrawingAtlasAddSquareFromIndex(cda, 4);
    int squareDirt = cubeDrawingAtlasAddSquareFromIndex(cda, 2);
    int squareStone = cubeDrawingAtlasAddSquareFromIndex(cda, 1);
    int squareTntTop = cubeDrawingAtlasAddSquareFromIndex(cda, 10);
    int squareTntSide = cubeDrawingAtlasAddSquareFromIndex(cda, 9);
    int squareTntBottom = cubeDrawingAtlasAddSquareFromIndex(cda, 11);
    int squareCrumble = cubeDrawingAtlasAddSquareFromRowCol(cda, 1, 0);
    int squareWood = cubeDrawingAtlasAddSquareFromRowCol(cda, 0, 5);
    int squareLogSide = cubeDrawingAtlasAddSquareFromRowCol(cda, 1, 4);
    int squareLogTop = cubeDrawingAtlasAddSquareFromRowCol(cda, 1, 5);
    int squareGoldTop = cubeDrawingAtlasAddSquareFromRowCol(cda, 1, 7);
    int squareGoldSide = cubeDrawingAtlasAddSquareFromRowCol(cda, 2, 7);
    int squareGoldBottom = cubeDrawingAtlasAddSquareFromRowCol(cda, 3, 7);

    _Static_assert(BLOCK_COUNT_ == 9, "add a drawing entry for the block(s) that are added and increment this expected value");
    cubeDrawingAtlasAddCube(cda, BLOCK_DIRT, makeBlockDrawingKind1(squareDirt));
    cubeDrawingAtlasAddCube(cda, BLOCK_GRASS, makeBlockDrawingKind1(squareGrass));
    cubeDrawingAtlasAddCube(cda, BLOCK_GRASSY_DIRT, makeBlockDrawingKind3(squareGrassSide, squareGrass, squareDirt));
    cubeDrawingAtlasAddCube(cda, BLOCK_STONE, makeBlockDrawingKind1(squareStone));
    cubeDrawingAtlasAddCube(cda, BLOCK_CRUMBLE, makeBlockDrawingKind1(squareCrumble));
    cubeDrawingAtlasAddCube(cda, BLOCK_WOOD, makeBlockDrawingKind1(squareWood));
    cubeDrawingAtlasAddCube(cda, BLOCK_LOG, makeBlockDrawingKind2(squareLogSide, squareLogTop));
    cubeDrawingAtlasAddCube(cda, BLOCK_TNT, makeBlockDrawingKind3(squareTntSide, squareTntTop, squareTntBottom));
    cubeDrawingAtlasAddCube(cda, BLOCK_GOLD, makeBlockDrawingKind3(squareGoldSide, squareGoldTop, squareGoldBottom));
}

// Return a `RayCollision` and write the `BlockPosition` of the hit to the pointer argument `blockPosResult` if a hit was found.
RayCollision rayCollisionBlocks(BlockPosition *blockPosResult) {
    Ray ray;
    ray.position = state->cam.position;
    ray.direction = Vector3Normalize(Vector3Subtract(state->cam.target, state->cam.position));

    RayCollision best = (RayCollision){0};
    best.hit = false;
    best.distance = INFINITY;

    BlockPosition blockHit;

    for (int i = 0; i < arrlen(state->daBlocks); i++) {
        BlockPair bp = state->daBlocks[i];
        BoundingBox blockBox;
        blockBox.min.x = bp.pos.x - 0.5f;
        blockBox.min.y = bp.pos.y - 0.5f;
        blockBox.min.z = bp.pos.z - 0.5f;
        blockBox.max.x = bp.pos.x + 0.5f;
        blockBox.max.y = bp.pos.y + 0.5f;
        blockBox.max.z = bp.pos.z + 0.5f;
        RayCollision rc = GetRayCollisionBox(ray, blockBox);
        if (rc.hit && rc.distance < best.distance) {
            best = rc;
            blockHit = bp.pos;
        }
    }

    if (best.hit) {
        *blockPosResult = blockHit;
    }

    return best;
}

int main() {
    // Zero-initialize the whole game state
    state = (FaceCraft*) malloc(sizeof(*state));
    memset(state, 0, sizeof(*state));

    float cubeSize = 1;

    float lookXSpeed = 0.1f;
    float lookYSpeed = 0.1f;

    BlockPosition targetBlockPos;
    bool validTargetBlockPos = false;

    // 2 selection block positions
    char nextBlockPos = 'a';
    BlockPosition selectedBlockPosA;
    BlockPosition selectedBlockPosB;
    bool validBlockPosA = false;
    bool validBlockPosB = false;

    // Initialize window and drawing
    InitWindow(820, 540, "Cube Tool");
    SetTargetFPS(60);

    initCubeDrawingAtlas(LoadTexture("texture.png"));

    // Init camera
    state->cam.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    state->cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    state->cam.target = (Vector3){ 0.0f, 2.0f, 4.0f };
    state->cam.fovy = 70.0f;
    state->cam.projection = CAMERA_PERSPECTIVE;

    // Place some blocks
    for (int x = 0; x < BLOCK_COUNT_; x++) {
        blocksSetBlockAtXYZ(&state->daBlocks, x, 1, 1, x);
    }
    {
        int squareBounds = 15;
        for (int x = -squareBounds; x <= squareBounds; x++) {
            for (int z = -squareBounds; z <= squareBounds; z++) {
                blocksSetBlockAtXYZ(&state->daBlocks, x, 0, z, BLOCK_GRASSY_DIRT);
            }
        }
    }
    // place a panel
    blocksSetBlockAtXYZ(&state->daPanels, -2, 2, -2, 2);

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

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // block breaking
            if (!validBlockPosA && !validBlockPosB) {
                RayCollision rc = rayCollisionBlocks(&selectedBlockPosA);
                if (rc.hit) {
                    validBlockPosA = true;
                }
                nextBlockPos = 'b';
            }
            else if (nextBlockPos == 'a') {
                RayCollision rc = rayCollisionBlocks(&selectedBlockPosA);
                if (rc.hit) {
                    validBlockPosA = true;
                }
                nextBlockPos = 'b';
            }
            else if (nextBlockPos == 'b') {
                RayCollision rc = rayCollisionBlocks(&selectedBlockPosB);
                if (rc.hit) {
                    validBlockPosB = true;
                }
                nextBlockPos = 'a';
            }
            /*
            validTargetBlockPos = false;
            RayCollision rc = rayCollisionBlocks(&targetBlockPos);
            if (rc.hit) {
                validTargetBlockPos = true;
                blocksDeleteBlockAt(&state->daBlocks, targetBlockPos);
            }
            */
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            // block placement
            validTargetBlockPos = false;
            RayCollision rc = rayCollisionBlocks(&targetBlockPos);
            if (rc.hit && rc.distance > 1.7f) {// this distance condition is a bad heuristic to prevent player from placing blocks on himself
                targetBlockPos.x += rc.normal.x;
                targetBlockPos.y += rc.normal.y;
                targetBlockPos.z += rc.normal.z;
                if (blocksGetIndexOfBlockAt(state->daBlocks, targetBlockPos) < 0) {
                    blocksSetBlockAt(&state->daBlocks, targetBlockPos, BLOCK_CRUMBLE);
                }
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
                // Draw blocks
                for (int i = 0; i < arrlen(state->daBlocks); ++i) {
                    BlockPair bp = state->daBlocks[i];
                    Vector3 centerPos = mapBlockPositionToVector3(bp.pos);
                    drawBlockKindAt(&state->cubeDrawAtlas, bp.blockDrawingKindIndex, centerPos, cubeSize, WHITE, -1);
                }

                for (int i = 0; i < arrlen(state->daPanels); ++i) {
                    BlockPair bp = state->daPanels[i];
                    Vector3 centerPos = mapBlockPositionToVector3(bp.pos);
                    UVPair uvPair = state->cubeDrawAtlas.daSquareUVs[bp.blockDrawingKindIndex];
                    drawCubeFaceTexture(state->cubeDrawAtlas.texture, uvPair, centerPos, 1.0f, WHITE, CUBE_DIRECTION_FRONT);
                }

                // Draw block wireframe
                if (validTargetBlockPos) {
                    Color color1 = WHITE;
                    color1.a /= 2;
                    DrawCubeWires(mapBlockPositionToVector3(targetBlockPos), 1.01f, 1.01f, 1.01f, color1);
                }
                // Draw block wireframe
                if (validBlockPosA) {
                    Color color1 = RED;
                    color1.a /= 2;
                    DrawCubeWires(mapBlockPositionToVector3(selectedBlockPosA), 1.01f, 1.01f, 1.01f, color1);
                }
                // Draw block wireframe
                if (validBlockPosB) {
                    Color color1 = BLUE;
                    color1.a /= 2;
                    DrawCubeWires(mapBlockPositionToVector3(selectedBlockPosB), 1.01f, 1.01f, 1.01f, color1);
                }
                if (validBlockPosA && validBlockPosB) {
                    Color color1 = WHITE;
                    Vector3 centerA = mapBlockPositionToVector3(selectedBlockPosA);
                    Vector3 centerB = mapBlockPositionToVector3(selectedBlockPosB);
                    DrawLine3D(centerA, centerB, color1);
                    Vector3 bCenter;
                    bCenter.x = (centerA.x + centerB.x) / 2.0f;
                    bCenter.y = (centerA.y + centerB.y) / 2.0f;
                    bCenter.z = (centerA.z + centerB.z) / 2.0f;
                    float w = 1 + fabsf(centerA.x - centerB.x);
                    float h = 1 + fabsf(centerA.y - centerB.y);
                    float l = 1 + fabsf(centerA.z - centerB.z);
                    DrawCubeWires(bCenter, w, h, l, color1);
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

            // Draw position text
            Camera *c = &state->cam;
            DrawText(TextFormat("Position: (%06.3f, %06.3f, %06.3f)\nLooking at: (%06.3f, %06.3f, %06.3f)",
                c->position.x, c->position.y, c->position.z,
                c->target.x, c->target.y, c->target.z),
                10, 10, 10, WHITE);
        }
        EndDrawing();
    }
    
    CloseWindow();
}