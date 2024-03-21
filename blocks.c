#include <assert.h>

#include "raylib/raylib.h"
#include "stb_ds.h"

#include "types.h"
#include "blocks.h"


BlockPosition makeBlockPosition(int x, int y, int z) {
    return (BlockPosition) { .x = x, .y = y, .z = z };
}

// Convert a `BlockPosition` to a raylib `Vector3`.
Vector3 mapBlockPositionToVector3(BlockPosition blockPos) {
    return (Vector3){ blockPos.x, blockPos.y, blockPos.z };
}

bool blockPositionEqual(BlockPosition a, BlockPosition b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

int blocksGetIndexOfBlockAt(BlockPair *daBlocks, BlockPosition blockPos) {
    for (int i = 0; i < arrlen(daBlocks); i++) {
        if (blockPositionEqual(daBlocks[i].pos, blockPos)) {
            return i;
        }
    }
    return -1;
}

// Set a block position to be a certain block kind within the map.
void blocksSetBlockAt(BlockPair **daBlocksPtr, BlockPosition blockPos, int blockKindIndex) {
    BlockPair *daBlocks = *daBlocksPtr;
    int index = blocksGetIndexOfBlockAt(daBlocks, blockPos);
    if (index >= 0) {
        // Block record already found, so update it.
        daBlocks[index].kindIndex = blockKindIndex;
    }
    else {
        // Block record not found, so add it.
        BlockPair newBlock = (BlockPair){0};
        newBlock.kindIndex = blockKindIndex;
        newBlock.pos = blockPos;
        arrput(daBlocks, newBlock);
    }
    // Pointer may have changed, so update it for the caller
    *daBlocksPtr = daBlocks;
}

BlockDrawingKind makeBlockKind(int allFacesInitialIndex) {
    BlockDrawingKind result = (BlockDrawingKind) { 0 };
    result.faces.topIndex = allFacesInitialIndex;
    result.faces.bottomIndex = allFacesInitialIndex;
    result.faces.frontIndex = allFacesInitialIndex;
    result.faces.backIndex = allFacesInitialIndex;
    result.faces.leftIndex = allFacesInitialIndex;
    result.faces.rightIndex = allFacesInitialIndex;
    return result;
}

int blockFacesGetByDirection(CubeFaces faces, CubeDirection d) {
    switch (d)
    {
    case CUBE_DIRECTION_TOP: return faces.topIndex;
    case CUBE_DIRECTION_BOTTOM: return faces.bottomIndex;
    case CUBE_DIRECTION_RIGHT: return faces.rightIndex;
    case CUBE_DIRECTION_LEFT: return faces.leftIndex;
    case CUBE_DIRECTION_FRONT: return faces.frontIndex;
    case CUBE_DIRECTION_BACK: return faces.backIndex;
    default: assert(0 && "invalid CubeDirection");
    }
    return faces.topIndex;
}