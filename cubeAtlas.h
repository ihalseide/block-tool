#ifndef _FACECRAFT_CUBEATLAS_H_DEFINED_
#define _FACECRAFT_CUBEATLAS_H_DEFINED_


#include "types.h"


UVPair makeUVPair(float u1, float v1, float u2, float v2);

// Initialize a CubeDrawingAtlas with a given grid size (rows, cols).
CubeDrawingAtlas makeCubeDrawingAtlas(Texture2D texture, int numRows, int numColumns);

// Initialize a CubeDrawingAtlas that tries to choose the grid size such that each grid rectangle is 16x16 pixels.
CubeDrawingAtlas makeCubeDrawingAtlas16(Texture2D texture);

// Add a UVPair square created to represent a tile at (row, col) to the atlas and return the index of it in the internal square array.
int cubeDrawingAtlasAddSquareFromRowCol(CubeDrawingAtlas *cda, int row, int col);

// Add a UVPair square to the atlas and return the index of it in the internal square array.
int cubeDrawingAtlasAddSquareFromIndex(CubeDrawingAtlas *cda, int index);

// Add a BlockDrawingKind cube to the atlas and return the index of it in the interal cube array.
int cubeDrawingAtlasAddCube(CubeDrawingAtlas *cda, BlockDrawingKind cube);

#endif // _FACECRAFT_CUBEATLAS_H_DEFINED_