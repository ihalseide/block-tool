#ifndef _FACECRAFT_CUBEATLAS_H_DEFINED_
#define _FACECRAFT_CUBEATLAS_H_DEFINED_


#include "types.h"


// Create a `UVPair` pair of 2 UV coordinates from the given points: `(u1, v1)`, and `(u2, v2)`.
UVPair makeUVPair(float u1, float v1, float u2, float v2);

// Initialize a `CubeDrawingAtlas` with a given grid size (rows x cols).
CubeDrawingAtlas makeCubeDrawingAtlas(Texture2D texture, int numRows, int numColumns);

// Frees the resources that are allocated by the `CubeDrawingAtlas`.
void freeCubeDrawingAtlas(CubeDrawingAtlas cda);

// Initialize a `CubeDrawingAtlas` that tries to choose the grid size such that each grid rectangle is 16x16 pixels.
// - NOTE: If the texture does not have a width or height that is not a multiple of 16 pixels,
// then all of the cube textures in this atlas will just look stretched. There will not be an "error".
CubeDrawingAtlas makeCubeDrawingAtlas16(Texture2D texture);

// Add a `UVPair` square created to represent a tile at (row, col) to the atlas and return the index of it in the internal square array.
int cubeDrawingAtlasAddSquareFromRowCol(CubeDrawingAtlas *cda, int row, int col);

// Add a `UVPair` square to the atlas and return the index of it in the internal square array.
int cubeDrawingAtlasAddSquareFromIndex(CubeDrawingAtlas *cda, int index);

// Add a `BlockDrawingKind` cube to the atlas B.D.K. hashmap
void cubeDrawingAtlasAddCube(CubeDrawingAtlas *cda, int key, BlockDrawingKind cube);

#endif // _FACECRAFT_CUBEATLAS_H_DEFINED_