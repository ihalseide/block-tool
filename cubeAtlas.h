#ifndef _FACECRAFT_CUBEATLAS_H_DEFINED_
#define _FACECRAFT_CUBEATLAS_H_DEFINED_


#include "types.h"


UVPair makeUVPair(float u1, float v1, float u2, float v2);

UVPair makeUVPairFromGridRowColumn(int row, int col, int numRows, int numColumns);

UVPair makeUVPairFromGridIndex(int index, int numRows, int numColumns);

// Initialize a CubeDrawingAtlas.
CubeDrawingAtlas makeCubeDrawingAtlas(Texture2D texture, int numRows, int numColumns);

// Add a UVPair square to the atlas and return the index of it in the internal square array.
int cubeDrawingAtlasAddSquare(CubeDrawingAtlas *cda, int squareRow, int squareColumn);

// Add a BlockDrawingKind cube to the atlas and return the index of it in the interal cube array.
int cubeDrawingAtlasAddCube(CubeDrawingAtlas *cda, BlockDrawingKind cube);


#endif // _FACECRAFT_CUBEATLAS_H_DEFINED_