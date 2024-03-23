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

    // Create squares
    _Static_assert(SQUARE_COUNT_ == 14, "all squares should be assigned below");
#define x(square, r, c) cubeDrawingAtlasSetSquareFromRowCol(cda, square, r, c)
    x(SQUARE_GRASS_SIDE, 0, 3);
    x(SQUARE_GRASS, 0, 4);
    x(SQUARE_DIRT, 0, 2);
    x(SQUARE_STONE, 0, 1);
    x(SQUARE_TNT_TOP, 0, 10);
    x(SQUARE_TNT_SIDE, 0, 9);
    x(SQUARE_TNT_BOTTOM, 0, 11);
    x(SQUARE_CRUMBLE, 1, 0);
    x(SQUARE_WOOD, 0, 5);
    x(SQUARE_LOG_SIDE, 1, 4);
    x(SQUARE_LOG_TOP, 1, 5);
    x(SQUARE_GOLD_TOP, 1, 7);
    x(SQUARE_GOLD_SIDE, 2, 7);
    x(SQUARE_GOLD_BOTTOM, 3, 7);
#undef defSquareRC

    _Static_assert(CUBE_COUNT_ == 10, "add a drawing entry for the block(s) that are added and increment this expected value");
    cubeDrawingAtlasAddCube(cda, CUBE_DIRT, makeBlockDrawingKind1(SQUARE_DIRT));
    cubeDrawingAtlasAddCube(cda, CUBE_GRASS, makeBlockDrawingKind1(SQUARE_GRASS));
    cubeDrawingAtlasAddCube(cda, CUBE_GRASSY_DIRT, makeBlockDrawingKind3(SQUARE_GRASS_SIDE, SQUARE_GRASS, SQUARE_DIRT));
    cubeDrawingAtlasAddCube(cda, CUBE_STONE, makeBlockDrawingKind1(SQUARE_STONE));
    cubeDrawingAtlasAddCube(cda, CUBE_CRUMBLE, makeBlockDrawingKind1(SQUARE_CRUMBLE));
    cubeDrawingAtlasAddCube(cda, CUBE_WOOD, makeBlockDrawingKind1(SQUARE_WOOD));
    cubeDrawingAtlasAddCube(cda, CUBE_LOG, makeBlockDrawingKind2(SQUARE_LOG_SIDE, SQUARE_LOG_TOP));
    cubeDrawingAtlasAddCube(cda, CUBE_TNT, makeBlockDrawingKind3(SQUARE_TNT_SIDE, SQUARE_TNT_TOP, SQUARE_TNT_BOTTOM));
    cubeDrawingAtlasAddCube(cda, CUBE_GOLD, makeBlockDrawingKind3(SQUARE_GOLD_SIDE, SQUARE_GOLD_TOP, SQUARE_GOLD_BOTTOM));
    cubeDrawingAtlasAddCube(cda, CUBE_TEST_BLOCK, makeBlockDrawingKind6(SQUARE_LOG_TOP, SQUARE_LOG_TOP, 0, 1, 2, 3));

}

BoundingBox getBlockBoundingBox(BlockPosition blockPos) {
    BoundingBox result = {0};
    result.min.x = blockPos.x - 0.5f;
    result.min.y = blockPos.y - 0.5f;
    result.min.z = blockPos.z - 0.5f;
    result.max.x = blockPos.x + 0.5f;
    result.max.y = blockPos.y + 0.5f;
    result.max.z = blockPos.z + 0.5f;
    return result;
}

// Return a `RayCollision` and write the `BlockPosition` of the hit to the pointer argument `blockPosResult` if a hit was found.
RayCollision rayCollisionBlocks(Ray ray, BlockPosition *blockPosResult) {
    BlockPosition blockHit = {0};
    RayCollision bestCollision = (RayCollision){0};
    bestCollision.distance = INFINITY;

    for (int i = 0; i < arrlen(state->daBlocks); i++) {
        BlockPair bp = state->daBlocks[i];
        BoundingBox blockBox = getBlockBoundingBox(bp.pos);
        RayCollision collide = GetRayCollisionBox(ray, blockBox);
        if (collide.hit && (collide.distance < bestCollision.distance)) {
            bestCollision = collide;
            blockHit = bp.pos;
        }
    }

    if (bestCollision.hit) {
        *blockPosResult = blockHit;
    }
    return bestCollision;
}

int main() {
    // Zero-initialize the whole game state
    state = (FaceCraft*) malloc(sizeof(*state));
    memset(state, 0, sizeof(*state));

    float cubeSize = 1;

    float lookXSpeed = 0.1f;
    float lookYSpeed = 0.1f;

    // cube editing
    BlockPosition targetBlockPos1;
    Vector3 targetNormal;
    bool validTargetBlock = false;

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
    for (int x = 0; x < CUBE_COUNT_; x++) {
        blocksSetBlockAtXYZ(&state->daBlocks, x, 1, 1, x);
    }
    {
        int squareBounds = 15;
        for (int x = -squareBounds; x <= squareBounds; x++) {
            for (int z = -squareBounds; z <= squareBounds; z++) {
                blocksSetBlockAtXYZ(&state->daBlocks, x, 0, z, CUBE_GRASSY_DIRT);
            }
        }
    }
    blocksSetBlockAtXYZ(&state->daBlocks, 3, 2, 3, CUBE_TEST_BLOCK);

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
            Ray ray;
            ray.position = state->cam.position;
            ray.direction = Vector3Normalize(Vector3Subtract(state->cam.target, state->cam.position));
            RayCollision blockCollision = rayCollisionBlocks(ray, &targetBlockPos1);
            validTargetBlock = blockCollision.hit;
            if (blockCollision.hit) {
                targetNormal = blockCollision.normal;
                int i = blocksGetIndexOfBlockAt(state->daBlocks, targetBlockPos1);
                assert(i >= 0);
                BlockPair bp = state->daBlocks[i];
                printf("clicked on cube #%d at (%d, %d, %d), normal (%1.1f,%1.1f,%1.1f)\n",
                    bp.blockDrawingKindIndex,
                    targetBlockPos1.x, targetBlockPos1.y, targetBlockPos1.z,
                    targetNormal.x, targetNormal.y, targetNormal.z);
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && validTargetBlock) {
            int i = blocksGetIndexOfBlockAt(state->daBlocks, targetBlockPos1);
            assert(i >= 0);
            BlockPair bp = state->daBlocks[i];
            if (state->cubeDrawAtlas.maxCubeId == bp.blockDrawingKindIndex) {
#if 0
                // cycle current face
                printf("cycle\n");
                CubeDirection faceSide = toCubeDirection(targetNormal);
                BDKEntry *entry = hmgetp(state->cubeDrawAtlas.hmCubeKinds, bp.blockDrawingKindIndex);
                BlockDrawingKind *bdk = &entry->value;
                assert(bdk);
                int *facePtr = blockFaceGetPtrByDirection(&bdk->faces, faceSide);
                int face = *facePtr;
                face++;
                if (face >= SQUARE_COUNT_) {
                    // wrap-around
                    face = 0;
                }
                *facePtr = face;
#else
                // rotate cube
                BDKEntry *entry = hmgetp(state->cubeDrawAtlas.hmCubeKinds, bp.blockDrawingKindIndex);
                assert(entry);
                BlockDrawingKind *bdk = &entry->value;
                assert(bdk);
                rotateCubeDrawingKindY(bdk);
#endif
            }
            else {
                // initialize
                int newBlockKind = duplicateCubeKind(&state->cubeDrawAtlas, bp.blockDrawingKindIndex);
                printf("created duplicate of cube kind #%d : #%d\n", bp.blockDrawingKindIndex, newBlockKind);
                blocksSetBlockAt(&state->daBlocks, targetBlockPos1, newBlockKind);
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

                // Draw block wireframe
                if (validTargetBlock) {
                    Vector3 center = mapBlockPositionToVector3(targetBlockPos1);
                    DrawCubeWires(center, 1.01f, 1.01f, 1.01f, WHITE);
                    DrawLine3D(center, Vector3Add(center, targetNormal), BLUE);
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
    
    // Cleanup
    freeCubeDrawingAtlas(state->cubeDrawAtlas);
    arrfree(state->daBlocks);
    free(state);
    
    CloseWindow();
}