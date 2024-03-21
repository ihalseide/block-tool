#ifndef _FACECRAFT_TYPES_H_INCLUDED_
#define _FACECRAFT_TYPES_H_INCLUDED_


#include <stdbool.h>
#include "raylib/raylib.h"


// Check that `x` is between `mini` and `maxi` (written as `mini <= x <= maxi` in Python)
#define BETWEEN(x, mini, maxi) ((mini <= x) && (x <= maxi))


// Represents each of the 6 possible cube directions or faces or normals
typedef enum CubeDirection {
    CUBE_DIRECTION_TOP,
    CUBE_DIRECTION_BOTTOM,
    CUBE_DIRECTION_RIGHT,
    CUBE_DIRECTION_LEFT,
    CUBE_DIRECTION_FRONT,
    CUBE_DIRECTION_BACK,
} CubeDirection;

// Collection of indices into a TextureAtlasGrid for the 6 faces of a cube.
typedef struct CubeFaces {
    int topIndex,
        bottomIndex,
        frontIndex,
        backIndex,
        leftIndex,
        rightIndex;
} CubeFaces;

// Collection of the properties for a kind of block for rendering.
typedef struct BlockDrawingKind {
    CubeFaces faces;
} BlockDrawingKind;

// A 3D block position.
typedef struct BlockPosition {
    int x, y, z;
} BlockPosition;

// A pair of a block position and a index into an array of BlockDrawingKind's
typedef struct BlockPair {
    BlockPosition pos;
    int blockDrawingKindIndex;
} BlockPair;

// Keep track of a pair of 2 UV coordinates (for texture coordinates on cube faces/"squares").
typedef struct UVPair {
    float u1, v1; // start/begin
    float u2, v2; // stop/end
} UVPair;

// Element type for a `hmap` of `int` -> `BlockDrawingKind`.
typedef struct BDKEntry {
    int key;
    BlockDrawingKind value;
} BDKEntry;

// Atlas that uses an array of cube faces that index into its array of UVPair's
typedef struct CubeDrawingAtlas {
    Texture2D texture; // texture/image that all of the cube and square members refer to
    int numColumns; // texture grid columns
    int numRows; // texture grid rows
    UVPair *daSquareUVs; // dynamic array of `UVPair`
    BDKEntry *hmCubeKinds; // hmap of `int` -> `BlockDrawingKind`
} CubeDrawingAtlas;

// Struct for all of the main program state.
typedef struct FaceCraft {
    CubeDrawingAtlas cubeDrawAtlas;
    Camera cam;
    BlockPair *daBlocks; // dynamic array of `BlockPair`s
} FaceCraft;

typedef enum BlockID {
    BLOCK_GRASS,
    BLOCK_GRASSY_DIRT,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_CRUMBLE,
    BLOCK_WOOD,
    BLOCK_LOG,
} BlockID;

#endif // _FACECRAFT_TYPES_H_INCLUDED_