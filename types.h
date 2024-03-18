#ifndef _FACECRAFT_TYPES_H_INCLUDED_
#define _FACECRAFT_TYPES_H_INCLUDED_


#include "raylib/raylib.h"


typedef struct CubeDirection {
    int d;
} CubeDirection;

typedef struct PanelKind {
    int id;
} PanelKind;

typedef struct BlockKind {
    int id;
} BlockKind;

typedef struct BlockPosition {
    int x, y, z;
} BlockPosition;

typedef struct Block {
    BlockKind kind;
    BlockPosition pos;
} Block;

typedef struct Panel {
    CubeDirection dir;
    PanelKind kind;
} Panel;

typedef struct Blocks {
    Block *da_start;
} Blocks;

typedef struct FaceCraft {
    Camera camera;
    Blocks blocks;
} FaceCraft;


#endif // _FACECRAFT_TYPES_H_INCLUDED_