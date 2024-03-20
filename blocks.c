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

bool isValidBlockKind(BlockKinds blockKinds, int blockKindIndex) {
    const BlockKind *da = blockKinds.da_start;
    int maxIndex = arrlen(da) - 1;
    return BETWEEN(blockKindIndex, 0, maxIndex);
}

BlockPair *mapGetBlockPointer(BlockMap *blockMap, BlockPosition blockPos) {
    for (int i = 0; i < arrlen(blockMap->blocks.da_start); i++) {
        BlockPair *b = &blockMap->blocks.da_start[i];
        if (blockPositionEqual(b->pos, blockPos)) {
            return b;
        }
    }
    return NULL;
}

const BlockPair *mapGetBlockPointer_const(const BlockMap *blockMap, BlockPosition blockPos) {
    for (int i = 0; i < arrlen(blockMap->blocks.da_start); i++) {
        const BlockPair *b = &blockMap->blocks.da_start[i];
        if (blockPositionEqual(b->pos, blockPos)) {
            return b;
        }
    }
    return NULL;
}

void mapSetBlock(BlockMap *blockMap, BlockPosition blockPos, int blockKindIndex) {
    if (!isValidBlockKind(blockMap->blockKinds, blockKindIndex)) {
        return;
    }
    BlockPair *foundBlock = mapGetBlockPointer(blockMap, blockPos);
    if (foundBlock) {
        // Block at given position already exists, so overwrite it.
        foundBlock->kindIndex = blockKindIndex;
    }
    else {
        // Block at given position does not yet exist, so create it.
        BlockPair newBlock = (BlockPair) { .kindIndex=blockKindIndex, .pos=blockPos };
        arrput(blockMap->blocks.da_start, newBlock);
    }
}

int mapGetBlockKindIndex(const BlockMap *blockMap, BlockPosition blockPos, int notFound) {
    const BlockPair *b = mapGetBlockPointer_const(blockMap, blockPos);
    if (b) {
        return b->kindIndex;
    }
    else {
        return notFound;
    }
}

void mapAddBlockKind(BlockMap *blockMap, BlockKind blockKind) {
    arrput(blockMap->blockKinds.da_start, blockKind);
}

const BlockKind *mapGetBlockKindAtPosition(const BlockMap *blockMap, BlockPosition blockPos) {
    const BlockPair *bp = mapGetBlockPointer_const(blockMap, blockPos);
    if (bp) {
        // There is a block at the given position.
        int kind = bp->kindIndex;
        assert(isValidBlockKind(blockMap->blockKinds, kind));
        const BlockKind *result = &blockMap->blockKinds.da_start[kind];
        return result;
    }
    else {
        // No block at the given position, so return NULL.
        return NULL;
    }
}

BlockKind makeBlockKind(int allFacesInitialIndex, Texture2D texture) {
    BlockKind result = (BlockKind) { 0 };
    result.texture = texture;
    result.faces.topIndex = allFacesInitialIndex;
    result.faces.bottomIndex = allFacesInitialIndex;
    result.faces.frontIndex = allFacesInitialIndex;
    result.faces.backIndex = allFacesInitialIndex;
    result.faces.leftIndex = allFacesInitialIndex;
    result.faces.rightIndex = allFacesInitialIndex;
    return result;
}

int blockFacesGetByDirection(BlockFaces faces, CubeDirection d) {
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