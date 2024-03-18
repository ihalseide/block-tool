#include <stdio.h>
#include "raylib/raylib.h"
#include "raylib/rlgl.h" 
#include "types.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"


// Main global game state (not directly accessible to the other source code files)
static FaceCraft *state;


void globalCleanup() {
    CloseWindow();
}

// Convert a `BlockPosition` to a raylib `Vector3`.
Vector3 mapBlockPositionToVector3(BlockPosition blockPos) {
    return (Vector3){ blockPos.x, blockPos.y, blockPos.z };
}

//------------------------------------------------------------------------------------
// Custom Functions Definition
//------------------------------------------------------------------------------------
// Draw cube textured
// NOTE: Cube position is the center position
// copied from: textured cube example https://www.raylib.com/examples.html
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // Vertex data transformation can be defined with the commented lines,
    // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
    //rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        //rlTranslatef(2.0f, 0.0f, 0.0f);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(2.0f, 2.0f, 2.0f);

        rlBegin(RL_QUADS);
            rlColor4ub(color.r, color.g, color.b, color.a);
            // Front Face
            rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            // Back Face
            rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            // Top Face
            rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            // Bottom Face
            rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            // Right face
            rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            // Left Face
            rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
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
    for (int j = 0; j < 100; j++)
    for (int i = 0; i < 100; i++) {
        new_block.pos = (BlockPosition){ i, 1, j };
        arrput(state->blocks.da_start, new_block);
    }

    Image img = LoadImage("texture.png");
    ImageFlipVertical(&img);
    Texture2D tex = LoadTextureFromImage(img);
    

    // Prepare to enter game loop
    DisableCursor();

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
                Color c = RED;
                c.a = 120;
                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLUE);
                DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);
                for (int i = 0; i < arrlen(state->blocks.da_start); ++i) {
                    Block b = state->blocks.da_start[i];
                    //DrawCube(mapBlockPositionToVector3(b.pos), 1, 1, 1, c);
                    DrawCubeTexture(tex, mapBlockPositionToVector3(b.pos), 1, 1, 1, WHITE);
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