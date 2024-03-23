#include <assert.h>
#include <stdio.h>

#include "raylib/raylib.h"
#include "stb_ds.h"

#include "types.h"
#include "blocks.h"


// Convert normal vector to cube direction (which side of the cube).
// Returns cube direction, or -1 if invalid.
CubeDirection toCubeDirection(Vector3 norm) {
    if (norm.x > 0) { return CUBE_DIRECTION_RIGHT; }
    if (norm.x < 0) { return CUBE_DIRECTION_LEFT; }
    if (norm.y > 0) { return CUBE_DIRECTION_TOP; }
    if (norm.y < 0) { return CUBE_DIRECTION_BOTTOM; }
    if (norm.z > 0) { return CUBE_DIRECTION_FRONT; }
    if (norm.z < 0) { return CUBE_DIRECTION_BACK; }
    return -1;
}

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

BlockPair blocksGetBlockAt(BlockPair *daBlocks, BlockPosition blockPos) {
    for (int i = 0; i < arrlen(daBlocks); i++) {
        if (blockPositionEqual(daBlocks[i].pos, blockPos)) {
            return daBlocks[i];
        }
    }
    return (BlockPair){0};
}

void blocksSetBlockAtXYZ(BlockPair **daBlocksPtr, int x, int y, int z, int blockKindIndex) {
    blocksSetBlockAt(daBlocksPtr, makeBlockPosition(x, y, z), blockKindIndex);
}

void blocksDeleteBlockAt(BlockPair **daBlocksPtr, BlockPosition blockPos) {
    BlockPair *daBlocks = *daBlocksPtr;
    int i = blocksGetIndexOfBlockAt(daBlocks, blockPos);
    if (i >= 0) {
        arrdelswap(daBlocks, i);
    }
    *daBlocksPtr = daBlocks;
}

// Set a block position to be a certain block kind within the map.
void blocksSetBlockAt(BlockPair **daBlocksPtr, BlockPosition blockPos, int blockKindIndex) {
    printf("setblock (%d, %d, %d) to %d\n", blockPos.x, blockPos.y, blockPos.z, blockKindIndex);
    BlockPair *daBlocks = *daBlocksPtr;
    int index = blocksGetIndexOfBlockAt(daBlocks, blockPos);
    if (index >= 0) {
        // Block record already found, so update it.
        daBlocks[index].blockDrawingKindIndex = blockKindIndex;
    }
    else {
        // Block record not found, so add it.
        BlockPair newBlock = (BlockPair){0};
        newBlock.blockDrawingKindIndex = blockKindIndex;
        newBlock.pos = blockPos;
        arrput(daBlocks, newBlock);
    }
    // Pointer may have changed, so update it for the caller
    *daBlocksPtr = daBlocks;
}

// Create a new BlockDrawingKind struct where each side is the same
BlockDrawingKind makeBlockDrawingKind1(int all) {
    return makeBlockDrawingKind6(all, all, all, all, all, all);
}

// Create a new BlockDrawingKind struct with the same sides, and the same top and bottom
BlockDrawingKind makeBlockDrawingKind2(int sides, int topAndBottom) {
    return makeBlockDrawingKind6(topAndBottom, topAndBottom, sides, sides, sides, sides);
}

// Create a new BlockDrawingKind struct with the same sides, but a different top and bottom.
BlockDrawingKind makeBlockDrawingKind3(int sides, int top, int bottom) {
    return makeBlockDrawingKind6(top, bottom, sides, sides, sides, sides);
}

// Create a new BlockDrawingKind struct with each side specified.
BlockDrawingKind makeBlockDrawingKind6(int top, int bottom, int left, int right, int front, int back) {
    BlockDrawingKind result = (BlockDrawingKind) { 0 };
    result.faces.topIndex = top;
    result.faces.bottomIndex = bottom;
    result.faces.rightIndex = right;
    result.faces.leftIndex = left;
    result.faces.frontIndex = front;
    result.faces.backIndex = back;
    return result;
}

int *blockFaceGetPtrByDirection(CubeFaces *faces, CubeDirection d) {
    switch (d)
    {
    case CUBE_DIRECTION_TOP: return &faces->topIndex;
    case CUBE_DIRECTION_BOTTOM: return &faces->bottomIndex;
    case CUBE_DIRECTION_RIGHT: return &faces->rightIndex;
    case CUBE_DIRECTION_LEFT: return &faces->leftIndex;
    case CUBE_DIRECTION_FRONT: return &faces->frontIndex;
    case CUBE_DIRECTION_BACK: return &faces->backIndex;
    default: assert(0 && "invalid CubeDirection");
    }
    return NULL;
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

void rotateCubeDrawingKindY(BlockDrawingKind *bdk) {
    int originalRightVal = bdk->faces.rightIndex;
    bdk->faces.rightIndex = bdk->faces.frontIndex;
    bdk->faces.frontIndex = bdk->faces.leftIndex;
    bdk->faces.leftIndex = bdk->faces.backIndex;
    bdk->faces.backIndex = originalRightVal;
}