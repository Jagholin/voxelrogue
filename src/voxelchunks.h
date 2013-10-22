#pragma once

#include <osg/Referenced>
#include <osg/ref_ptr>
#include <map>

struct VoxelData
{
    unsigned short int vId;
    bool operator ==(const VoxelData& rhs) const 
    {
        return vId == rhs.vId;
    }
    bool operator <(const VoxelData& rhs) const
    {
        return vId < rhs.vId;
    }
};

class VoxelChunk : public osg::Referenced
{
public:
    virtual ~VoxelChunk() {}
    virtual VoxelChunk* morph() {return this;}

    virtual VoxelChunk* setVoxelData(unsigned int x, unsigned int y, unsigned int z, const VoxelData &data) = 0;
    virtual void voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out) = 0;
};

class VoxelEmptyChunk : public VoxelChunk
{
public:
    VoxelEmptyChunk(VoxelData chunkData);
    // virtual VoxelChunk* morph(); // << can't do it here, voxel population and change functions should do this

    virtual VoxelChunk* setVoxelData(unsigned int x, unsigned int y, unsigned int z, const VoxelData &data); 
    virtual void voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out);

protected:
    VoxelData m_chunkData;
};

class VoxelGenericChunk: public VoxelChunk
{
public:
    VoxelGenericChunk(VoxelData fillData);
    virtual VoxelChunk* morph();

    virtual VoxelChunk* setVoxelData(unsigned int x, unsigned int y, unsigned int z, const VoxelData &data);
    virtual void voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out);

protected:
    VoxelData m_voxels[32*32*32];
    std::map<VoxelData, unsigned int> m_statistics;
};


class VoxelChunkWrapper : public osg::Referenced
{
public:
    VoxelChunkWrapper(VoxelData initData);
    void morphTick();

    void setVoxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData const& rhs);
    void voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out);

    bool isTesselDirty();
    void clearTesselDirty();
protected:
    osg::ref_ptr<VoxelChunk> m_realChunk;
    bool m_tesselDirty;
};

VoxelChunkWrapper* createRandomChunk();