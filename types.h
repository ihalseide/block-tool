#ifndef _FACECRAFT_TYPES_H_INCLUDED_
#define _FACECRAFT_TYPES_H_INCLUDED_


#include <stdbool.h>
#include "raylib/raylib.h"


typedef enum CubeDirection {
    CUBE_DIRECTION_TOP = 1,
    CUBE_DIRECTION_BOTTOM,
    CUBE_DIRECTION_LEFT,
    CUBE_DIRECTION_RIGHT,
    CUBE_DIRECTION_FRONT,
    CUBE_DIRECTION_BACK,
} CubeDirection;

typedef struct BlockFaces {
    int topIndex;
    int bottomIndex;
    int frontIndex;
    int backIndex;
    int leftIndex;
    int rightIndex;
} BlockFaces;

typedef struct BlockKind {
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

typedef struct FaceCraft {
    Camera cam;
    BlockMap map;
} FaceCraft;


#endif // _FACECRAFT_TYPES_H_INCLUDED_