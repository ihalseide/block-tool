#ifndef _FACECRAFT_TYPES_H_INCLUDED_
#define _FACECRAFT_TYPES_H_INCLUDED_


#include <stdbool.h>
#include "raylib/raylib.h"


#define BETWEEN(x, mini, maxi) ((mini <= x) && (x <= maxi))


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

typedef struct UVPair {
    float u1, v1; // start/begin
    float u2, v2; // stop/end
} UVPair;

// Atlas that uses an array of cube faces that index into its TextureAtlasGrid's list of UVPair's
typedef struct CubeDrawingAtlas {
    Texture2D texture;
    int numColumns; // texture grid columns
    int numRows; // texture grid rows
    UVPair *daSquareUVs; // dynamic array
    BlockDrawingKind *daCubeKinds; // dynamic array
} CubeDrawingAtlas;

// Struct for all of the main program state.
typedef struct FaceCraft {
    CubeDrawingAtlas cubeDrawAtlas;
    Camera cam;
    BlockPair *daBlocks; // dynamic array of `BlockPair`s
} FaceCraft;


#endif // _FACECRAFT_TYPES_H_INCLUDED_