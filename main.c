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

UVPair makeUVPair(float u1, float v1, float u2, float v2) {
    return (UVPair) { .u1 = u1, .v1 = v1, .u2 = u2, .v2 = v2 };
}

UVPair makeUVPairFromGridIndex(int index, int gridWidth, int gridHeight) {
    assert(index >= 0);
    assert(gridWidth > 0);
    assert(gridHeight > 0);
    float x = index % gridWidth;
    float y = index / gridWidth; // meant to be a round number
    return makeUVPair(x / gridWidth, y / gridHeight, (x + 1) / gridWidth, (y + 1) / gridHeight);
}

int addTextureUVs(AtlasUVs *textureUVs, UVPair uvs) {
    int indexResult = arrlen(textureUVs->da_start);
    arrput(textureUVs->da_start, uvs);
    return indexResult;
}

bool isValidTextureUVsIndex(int index) {
    int max_i = arrlen(state->blockTextureUVs.da_start) - 1;
    return BETWEEN(index, 0, max_i);
}

bool defineBlockKind(BlockKind kind) {
    if (!isValidTextureUVsIndex(kind.faces.topIndex)) { return false; }
    if (!isValidTextureUVsIndex(kind.faces.bottomIndex)) { return false; }
    if (!isValidTextureUVsIndex(kind.faces.rightIndex)) { return false; }
    if (!isValidTextureUVsIndex(kind.faces.leftIndex)) { return false; }
    if (!isValidTextureUVsIndex(kind.faces.frontIndex)) { return false; }
    if (!isValidTextureUVsIndex(kind.faces.backIndex)) { return false; }

    mapAddBlockKind(&state->map, kind);

    return true;
}

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
    int textureIndex1 = addTextureUVs(&state->blockTextureUVs, makeUVPairFromGridIndex(1, 2, 2));
    int textureIndex2 = addTextureUVs(&state->blockTextureUVs, makeUVPairFromGridIndex(2, 2, 2));
    int textureIndex3 = addTextureUVs(&state->blockTextureUVs, makeUVPairFromGridIndex(3, 2, 2));
    BlockKind grass = makeBlockKind(textureIndex2, tex);
    grass.faces.topIndex = textureIndex3;
    grass.faces.bottomIndex = textureIndex1;
    assert(defineBlockKind(grass));
    mapSetBlock(&state->map, makeBlockPosition(1, 1, 1), 0);
    mapSetBlock(&state->map, makeBlockPosition(1, 5, 1), 0);
    printf("There are %ld block kinds\n", arrlen(state->map.blockKinds.da_start));
    printf("There are %ld blocks\n", arrlen(state->map.blocks.da_start));

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
                    BlockPair bp = state->map.blocks.da_start[i];
                    //DrawCube(mapBlockPositionToVector3(b.pos), 1, 1, 1, c);
                    //drawCubeTexture(tex, mapBlockPositionToVector3(b.pos), 0.95f, 0.95f, 0.95f, WHITE);
                    //drawCubeFaceTexture(tex, mapBlockPositionToVector3(b.pos), 1, WHITE, i % (CUBE_DIRECTION_BACK+1));
                    BlockKind kind = *mapGetBlockKindAtPosition(&state->map, bp.pos);
                    drawBlockKindAt(&state->blockTextureUVs, kind, mapBlockPositionToVector3(bp.pos), 0.95f, WHITE, -1);
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