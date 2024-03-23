// Functions for working with the `CubeDrawingAtlas` data structure.

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
static void cubeDrawingAtlasAddUVSquare(CubeDrawingAtlas *cda, int index, UVPair square) {
    hmput(cda->hmSquareUVs, index, square);
}

CubeDrawingAtlas makeCubeDrawingAtlas(Texture2D texture, int numRows, int numColumns) {
    CubeDrawingAtlas result = (CubeDrawingAtlas) { 0 };
    result.hmCubeKinds = NULL;
    result.hmSquareUVs = NULL;
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
    hmfree(cda.hmCubeKinds);
    arrfree(cda.hmSquareUVs);
}

void cubeDrawingAtlasSetSquareFromRowCol(CubeDrawingAtlas *cda, int squareId, int row, int col) {
    cubeDrawingAtlasAddUVSquare(cda, squareId,
        makeUVPairFromGridRowColumn(row, col, cda->numRows, cda->numColumns));
}

void cubeDrawingAtlasAddSquareFromIndex(CubeDrawingAtlas *cda, int squareId, int index) {
    cubeDrawingAtlasAddUVSquare(cda, squareId,
        makeUVPairFromGridIndex(index, cda->numRows, cda->numColumns));
}

void cubeDrawingAtlasAddCube(CubeDrawingAtlas *cda, int key, BlockDrawingKind cube) {
    hmput(cda->hmCubeKinds, key, cube);
    if (key > cda->maxCubeId) {
        cda->maxCubeId = key;
    }
}

// Get if the given cubeId is a custom cube kind.
bool isCustomCubeKind(int cubeId) {
    return cubeId >= CUBE_COUNT_;
}

// Create a new cube kind that is a duplicate of the given cubeId and return the new id.
int duplicateCubeKind(CubeDrawingAtlas *cda, int cubeId) {
    BlockDrawingKind bdk = hmget(cda->hmCubeKinds, cubeId);
    int newId = cda->maxCubeId + 1;
    cubeDrawingAtlasAddCube(cda, newId, bdk);
    return newId;
}