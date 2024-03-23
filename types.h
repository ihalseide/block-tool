#ifndef _FACECRAFT_TYPES_H_INCLUDED_
#define _FACECRAFT_TYPES_H_INCLUDED_


#include <stdbool.h>
#include "raylib/raylib.h"


// Check that `x` is between `mini` and `maxi` (written as `mini <= x <= maxi` in Python)
#define BETWEEN(x, mini, maxi) ((mini <= x) && (x <= maxi))

#define MIN(a, b) (((a) < (b))? (a) : (b))

#define MAX(a, b) (((a) > (b))? (a) : (b))


// Represents each of the 6 possible cube directions or faces or normals
typedef enum CubeDirection {
    CUBE_DIRECTION_TOP,
    CUBE_DIRECTION_BOTTOM,
    CUBE_DIRECTION_RIGHT,
    CUBE_DIRECTION_LEFT,
    CUBE_DIRECTION_FRONT,
    CUBE_DIRECTION_BACK,
} CubeDirection;

// Block face IDs
typedef enum SquareID {
    SQUARE_CRUMBLE,
    SQUARE_GRASS,
    SQUARE_DIRT,
    SQUARE_GRASS_SIDE,
    SQUARE_WOOD,
    SQUARE_LOG_TOP,
    SQUARE_LOG_SIDE,
    SQUARE_GOLD_SIDE,
    SQUARE_GOLD_TOP,
    SQUARE_GOLD_BOTTOM,
    SQUARE_STONE,
    SQUARE_TNT_TOP,
    SQUARE_TNT_BOTTOM,
    SQUARE_TNT_SIDE,
    SQUARE_COUNT_, // Do not add new block below SQUARE_COUNT_, add them above instead!
} SquareID;

// The Block IDs
typedef enum CubeID {
    CUBE_GRASS,
    CUBE_GRASSY_DIRT,
    CUBE_DIRT,
    CUBE_STONE,
    CUBE_CRUMBLE,
    CUBE_WOOD,
    CUBE_LOG,
    CUBE_TNT,
    CUBE_GOLD,
    CUBE_TEST_BLOCK,
    CUBE_COUNT_, // Do not add new block below CUBE_COUNT_, add them above instead!
} CubeID;

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

// Element type for a `hmap` of `int` -> `UVPair`.
typedef struct UVPEntry {
    int key;
    UVPair value;
} UVPEntry;

// Element type for a `hmap` of `int` -> `BlockDrawingKind`.
typedef struct BDKEntry {
    int key;
    BlockDrawingKind value;
} BDKEntry;

// Atlas that uses an array of cube faces that index into its array of UVPair's
typedef struct CubeDrawingAtlas {
    Texture2D texture;  // texture/image that all of the cube and square members refer to
    int numColumns;  // texture grid columns
    int numRows;  // texture grid rows
    UVPEntry *hmSquareUVs;  // hmap of `int` -> `UVPair`
    BDKEntry *hmCubeKinds;  // hmap of `int` -> `BlockDrawingKind`
    int maxCubeId;  // should have the maximum cube id used as a key in `hmCubeKinds`
} CubeDrawingAtlas;

// Struct for all of the main program state.
typedef struct FaceCraft {
    CubeDrawingAtlas cubeDrawAtlas;
    Camera cam;
    BlockPair *daBlocks;  // dynamic array of `BlockPair`s for solid cubes
    BlockPair *daFascade;  // dynamic array of `BlockPair`s for panels / lone faces
} FaceCraft;

#endif // _FACECRAFT_TYPES_H_INCLUDED_