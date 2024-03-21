#ifndef _FACECRAFT_BLOCKS_H_INCLUDED_
#define _FACECRAFT_BLOCKS_H_INCLUDED_


#include "types.h"

// Create a new Block Position struct
BlockPosition makeBlockPosition(int x, int y, int z);

// Create a new BlockDrawingKind struct with an initial index that is the same for all of the faces.
BlockDrawingKind makeBlockDrawingKind1(int allFacesInitialIndex);

// Create a new BlockDrawingKind struct with the same sides, and the same top and bottom
BlockDrawingKind makeBlockDrawingKind2(int sides, int topAndBottom);

// Create a new BlockDrawingKind struct with the same sides, but a different top and bottom.
BlockDrawingKind makeBlockDrawingKind3(int sides, int top, int bottom);

// Create a new BlockDrawingKind struct by specifying all of the face indices.
BlockDrawingKind makeBlockDrawingKind6(int top, int bottom, int left, int right, int front, int back);

// Check if 2 Block Positions are equal
bool blockPositionEqual(BlockPosition a, BlockPosition b);

// Get the index of a block in the map at the given block position.
// Returns: index into `daBlocks` or -1 if not found.
// NOTE: the position for the returned Block should NOT be modified!
int blocksGetIndexOfBlockAt(BlockPair *daBlocks, BlockPosition blockPos);

// Set a block position to be a certain block kind within the map.
// - `daBlocksPtr`: is a pointer to a dynamic array of BlockPairs.
void blocksSetBlockAt(BlockPair **daBlocksPtr, BlockPosition blockPos, int blockKindIndex);

// Set a block position (X,Y,Z) to be a certain block kind within the map.
// - `daBlocksPtr`: is a pointer to a dynamic array of BlockPairs.
void blocksSetBlockAtXYZ(BlockPair **daBlocksPtr, int x, int y, int z, int blockKindIndex);

// Delete/remove the block (if any) at the given position.
void blocksDeleteBlockAt(BlockPair **daBlocksPtr, BlockPosition blockPos);

// Map/convert from a `BlockPosition` to a `Vector3`
Vector3 mapBlockPositionToVector3(BlockPosition blockPos);

// Get the block face field corresponding to the cube direction.
int blockFacesGetByDirection(CubeFaces faces, CubeDirection d);


#endif //_FACECRAFT_BLOCKS_H_INCLUDED_