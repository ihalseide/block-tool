#include <assert.h>

#include "stb_ds.h"

#include "types.h"
#include "cubeAtlas.h"


UVPair makeUVPair(float u1, float v1, float u2, float v2) {
    return (UVPair) { .u1 = u1, .v1 = v1, .u2 = u2, .v2 = v2 };
}

// Private helper function
static UVPair makeUVPairFromGridRowColumn(int row, int col, int numRows, int numColumns) {
    assert(row >= 0);
    assert(row < numRows);
    assert(col >= 0);
    assert(col < numColumns);
    float u1 = (float) col / numColumns;
    float v1 = (float) row / numRows;
    float u2 = (float) (col + 1) / numColumns;
    float v2 = (float) (row + 1) / numRows;
    return makeUVPair(u1, v1, u2, v2);
}

// Private helper function
static UVPair makeUVPairFromGridIndex(int index, int numRows, int numColumns) {
    assert(index >= 0);
    assert(numColumns > 0);
    assert(numRows > 0);
    assert(index < numColumns * numRows);
    int col = index % numColumns; // a.k.a. `X`
    int row = index / numColumns; // a.k.a. `Y`
    return makeUVPairFromGridRowColumn(row, col, numRows, numColumns);
}

// Private helper function
// Add a UVPair square to the atlas and return the index of it in the internal square array.
static int cubeDrawingAtlasAddUVSquare(CubeDrawingAtlas *cda, UVPair square) {
    int resultIndex = arrlen(cda->daSquareUVs);
    arrput(cda->daSquareUVs, square);
    return resultIndex;
}

CubeDrawingAtlas makeCubeDrawingAtlas(Texture2D texture, int numRows, int numColumns) {
    CubeDrawingAtlas result = (CubeDrawingAtlas) { 0 };
    result.texture = texture;
    result.numRows = numRows;
    result.numColumns = numColumns;
    return result;
}

CubeDrawingAtlas makeCubeDrawingAtlas16(Texture2D texture) {
    int numRows = texture.height / 16;
    int numCols = texture.width / 16;
    return makeCubeDrawingAtlas(texture, numRows, numCols);
}

void freeCubeDrawingAtlas(CubeDrawingAtlas cda) {
    free(cda.daCubeKinds);
    free(cda.daSquareUVs);
}

int cubeDrawingAtlasAddSquareFromRowCol(CubeDrawingAtlas *cda, int row, int col) {
    return cubeDrawingAtlasAddUVSquare(cda,
        makeUVPairFromGridRowColumn(row, col, cda->numRows, cda->numColumns));
}

int cubeDrawingAtlasAddSquareFromIndex(CubeDrawingAtlas *cda, int index) {
    return cubeDrawingAtlasAddUVSquare(cda,
        makeUVPairFromGridIndex(index, cda->numRows, cda->numColumns));
}

int cubeDrawingAtlasAddCube(CubeDrawingAtlas *cda, BlockDrawingKind cube) {
    int resultIndex = arrlen(cda->daCubeKinds);
    arrput(cda->daCubeKinds, cube);
    return resultIndex;
}