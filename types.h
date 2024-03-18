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

typedef struct Block {
    BlockKind kind;
} Block;

typedef struct Panel {
    CubeDirection dir;
    PanelKind kind;
} Panel;

typedef struct FaceCraft {
    Camera camera;
} FaceCraft;


#endif // _FACECRAFT_TYPES_H_INCLUDED_