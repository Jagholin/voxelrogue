#pragma once 

#include <osg/Drawable>
#include <osg/Group>
#include <vector>
#include "voxelchunks.h"

class VoxelCursor;

class ChunkGraphNode : public osg::Referenced
{
public:
    enum Directions {
        SOUTH = 0, EAST, NORTH, WEST, TOP, BOTTOM
    };

    ChunkGraphNode(osg::Vec3i chunkCoord, osg::ref_ptr<VoxelChunkWrapper> const& chunk);
    void loadChunkIn(osg::Vec3i chunkCoord, Directions at);
    VoxelChunkWrapper* getChunk();

    ChunkGraphNode* getNeighbour(Directions at);
    ChunkGraphNode* getOrLoadNeighbour(Directions at);
    void unloadNeighbour(Directions at);
    ChunkGraphNode* getExistingNeigbour(Directions &at);

    static Directions opposite(Directions at);

    VoxelCursor* getCursorInChunk(osg::Vec3i insideCoords);

protected:
    osg::ref_ptr<VoxelChunkWrapper> m_chunk;
    osg::ref_ptr<ChunkGraphNode> m_neighbours[6];
    osg::Vec3i m_chunkCoord;
};

class VoxelTerrainChunkColumn : public osg::Drawable
{
public:
    VoxelTerrainChunkColumn();
    VoxelTerrainChunkColumn(const VoxelTerrainChunkColumn& rhs, const osg::CopyOp& op);
    META_Object(voxel, VoxelTerrainChunkColumn);

    virtual osg::BoundingBox computeBound() const;

    virtual void drawImplementation( osg::RenderInfo& renderInfo ) const;

    friend struct TesselatorUpdateCallback;
protected:
    void tesselateChunk(osg::ref_ptr<VoxelChunkWrapper> const& chunk);
protected:
    std::vector<osg::ref_ptr<ChunkGraphNode> > m_chunks;
};

class VoxelTerrainGroup: public osg::Node
{
public:
    VoxelTerrainGroup(const VoxelTerrainGroup& rhs, const osg::CopyOp& op);
    META_Node(voxel, VoxelTerrainGroup);
 
protected:
    std::vector<osg::ref_ptr<VoxelTerrainChunkColumn>> m_drawables;
};

class VoxelCursor
{
public:
    VoxelCursor(ChunkGraphNode *chunk, osg::Vec3i coord = osg::Vec3i());

    void move(unsigned int dx, unsigned int dy, unsigned int dz);
    void move(osg::Vec3i dv);

    VoxelData getCurrentVoxel();
    VoxelData getNeighbourVoxel(osg::Vec3i dv);
protected:
    osg::Vec3i m_voxelCoords;
    ChunkGraphNode m_chunk;
    //VoxelTerrainGroup *m_terrain;
};