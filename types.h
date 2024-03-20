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

typedef struct BlockFaces {
    // indices into an array of texture UVs for each face
    int topIndex;
    int bottomIndex;
    int frontIndex;
    int backIndex;
    int leftIndex;
    int rightIndex;
} BlockFaces;

typedef struct BlockKind {
    Texture2D texture;
    BlockFaces faces;
} BlockKind;

typedef struct BlockPosition {
    int x, y, z;
} BlockPosition;

typedef struct BlockPair {
    BlockPosition pos;
    int kindIndex;
} BlockPair;

typedef struct Blocks {
    BlockPair *da_start;
} Blocks;

typedef struct BlockKinds {
    BlockKind *da_start;
} BlockKinds;

typedef struct BlockMap {
    BlockKinds blockKinds;
    Blocks blocks;
} BlockMap;

typedef struct UVPair {
    float u1, v1; // start/begin
    float u2, v2; // stop/end
} UVPair;

typedef struct AtlasUVs {
    UVPair *da_start;
} AtlasUVs;

typedef struct FaceCraft {
    Camera cam;
    BlockMap map;
    AtlasUVs blockTextureUVs;
} FaceCraft;


#endif // _FACECRAFT_TYPES_H_INCLUDED_