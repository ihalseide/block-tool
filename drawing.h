#ifndef _FACECRAFT_DRAWING_H_DEFINED_
#define _FACECRAFT_DRAWING_H_DEFINED_

#include "raylib/raylib.h"

#include "types.h"


// Draw cube face textured
void drawCubeFaceTexture(Texture2D texture, Vector3 centerPos, float size, Color color, CubeDirection faceSide);

// Draw cube textured
// NOTE: Cube position is the center position
// copied from: textured cube example https://www.raylib.com/examples.html
void drawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);


#endif // _FACECRAFT_DRAWING_H_DEFINED_