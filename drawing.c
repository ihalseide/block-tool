#include <assert.h>

#include "blocks.h"
#include "drawing.h"

#include "stb_ds.h"

#include "raylib/rlgl.h"


void drawBlockKindAt(const AtlasUVs *textureUVs, BlockKind blockKind, Vector3 centerPos, float size, Color color, long facesFlags) {
    const int max_i = arrlen(textureUVs->da_start);
    for (CubeDirection dir = 0; dir < 6; dir++) {
        if (facesFlags & (1 << dir)) {
            int lookupIndex = blockFacesGetByDirection(blockKind.faces, dir);
            assert(BETWEEN(lookupIndex, 0, max_i));
            UVPair uvPair = textureUVs->da_start[lookupIndex];
            drawCubeFaceTexture(blockKind.texture, uvPair, centerPos, size, color, dir);
        }
    }
}

void drawCubeFaceTexture(Texture2D texture, UVPair uvPair, Vector3 centerPos, float size, Color color, CubeDirection faceSide) {
    const float u1 = uvPair.u1;
    const float v1 = uvPair.v1;
    const float u2 = uvPair.u2;
    const float v2 = uvPair.v2;
    const float x = centerPos.x;
    const float y = centerPos.y;
    const float z = centerPos.z;
    const float width = size;
    const float height = size;
    const float length = size;
    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);
    switch (faceSide)
    {
    case CUBE_DIRECTION_TOP:
        // Top Face
        rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
        rlTexCoord2f(u1, v2); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlTexCoord2f(u1, v1); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
        rlTexCoord2f(u2, v1); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
        rlTexCoord2f(u2, v2); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
        break;
    case CUBE_DIRECTION_BOTTOM:
        // Bottom Face
        rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
        rlTexCoord2f(u2, v2); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
        rlTexCoord2f(u1, v2); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlTexCoord2f(u1, v1); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
        rlTexCoord2f(u2, v1); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
        break;
    case CUBE_DIRECTION_LEFT:
        // Left Face
        rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
        rlTexCoord2f(u1, v2); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
        rlTexCoord2f(u2, v2); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
        rlTexCoord2f(u2, v1); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
        rlTexCoord2f(u1, v1); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        break;
    case CUBE_DIRECTION_RIGHT:
        // Right face
        rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
        rlTexCoord2f(u2, v2); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
        rlTexCoord2f(u2, v1); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
        rlTexCoord2f(u1, v1); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
        rlTexCoord2f(u1, v2); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
        break;
    case CUBE_DIRECTION_FRONT:
        // Front Face
        rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
        rlTexCoord2f(u1, v2); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
        rlTexCoord2f(u2, v2); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
        rlTexCoord2f(u2, v1); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
        rlTexCoord2f(u1, v1); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
        break;
    case CUBE_DIRECTION_BACK:
        // Back Face
        rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
        rlTexCoord2f(u2, v2); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
        rlTexCoord2f(u2, v1); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
        rlTexCoord2f(u1, v1); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlTexCoord2f(u1, v2); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
        break;
    default:
        break;
    }
    rlEnd();
    rlSetTexture(0);
}

// Draw cube textured
// NOTE: Cube position is the center position
// copied from: textured cube example https://www.raylib.com/examples.html
void drawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
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