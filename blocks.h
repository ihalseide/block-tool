#ifndef _FACECRAFT_BLOCKS_H_INCLUDED_
#define _FACECRAFT_BLOCKS_H_INCLUDED_


#include "types.h"

// Create a new Block Position struct
BlockPosition makeBlockPosition(int x, int y, int z);

// Create a new BlockKind data value with an initial index that is the same for all of the faces.
BlockKind makeBlockKind(int allFacesInitialIndex);

// Check if 2 Block Positions are equal
bool blockPositionEqual(BlockPosition a, BlockPosition b);

// Add or "register" a new block kind to the block map.
// Returns the index/id of the new block kind, or the failValue if failed.
void mapAddBlockKind(BlockMap *blockMap, BlockKind blockKind);

// Check if a given blockKindIndex is valid (found inside the given blockKinds)
bool isValidBlockKind(BlockKinds blockKinds, int blockKindIndex);

// Get a pointer to a block in the map at the given position.
// NOTE: the position for the returned Block should NOT be modified!
BlockPair *mapGetBlockPointer(BlockMap *blockMap, BlockPosition blockPos);

// Get a const pointer to a block in the map at the given position.
// (Is the `const` version of the `mapGetBlockPointer` function.)
const BlockPair *mapGetBlockPointer_const(const BlockMap *blockMap, BlockPosition blockPos);

// Set a block position to be a certain block kind within the map.
void mapSetBlock(BlockMap *blockMap, BlockPosition blockPos, int blockKindIndex);

// Get the integer kind index of the block found at the given Block Position, or return `notFound` if there is no block there.
int mapGetBlockKindIndex(const BlockMap *blockMap, BlockPosition blockPos, int notFound);

// Get the Block Kind information at a given block position.
const BlockKind *mapGetBlockKindAtPosition(const BlockMap *blockMap, BlockPosition blockPos);

// Map/convert from a `BlockPosition` to a `Vector3`
Vector3 mapBlockPositionToVector3(BlockPosition blockPos);


#endif //_FACECRAFT_BLOCKS_H_INCLUDED_