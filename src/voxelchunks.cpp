#include "voxelchunks.h"
#include <cmath>
#include <cassert>

// The implementation of voxelChunks is now incredibly unoptimized, but i don't care much
// (createRandomChunk() function runtime is ~2ms in Release mode)

VoxelEmptyChunk::VoxelEmptyChunk(VoxelData chunkData):
    m_chunkData(chunkData)
{
    // nop
}

VoxelChunk* VoxelEmptyChunk::setVoxelData(unsigned int x, unsigned int y, unsigned int z, const VoxelData &data)
{
    if (data == m_chunkData)
        return this;

    VoxelGenericChunk * newChunk = new VoxelGenericChunk(m_chunkData);
    return newChunk->setVoxelData(x, y, z, data);
}

void VoxelEmptyChunk::voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out)
{
    out = m_chunkData;
}

VoxelGenericChunk::VoxelGenericChunk(VoxelData fillData)
{
    for (unsigned int i = 0; i < 32*32*32; ++i)
        m_voxels[i] = fillData;

    m_statistics[fillData] = 32*32*32;
}

VoxelChunk* VoxelGenericChunk::morph()
{
    if (m_statistics.size() == 1 && m_statistics.begin()->second == 32*32*32)
    {
        return new VoxelEmptyChunk(m_voxels[0]);
    }

    return this;
}

VoxelChunk* VoxelGenericChunk::setVoxelData(unsigned int x, unsigned int y, unsigned int z, const VoxelData &data)
{
    if (m_voxels[x*32*32 + y*32 + z] == data)
        return this;

    VoxelData previousData = m_voxels[x*32*32 + y*32 + z];
    m_voxels[x*32*32 + y*32 + z] = data;
    
    std::map<VoxelData, unsigned int>::iterator prevVoxel, newVoxel;
    prevVoxel = m_statistics.find(previousData);

    assert (prevVoxel != m_statistics.end());
    -- (prevVoxel->second);
    if (prevVoxel->second == 0)
        m_statistics.erase(prevVoxel);

    newVoxel = m_statistics.find(data);
    if (newVoxel == m_statistics.end())
        m_statistics[data] = 1;
    else
        ++ (newVoxel->second);

    return this;
}

void VoxelGenericChunk::voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out)
{
    out = m_voxels[x*32*32 + y*32 + z*32];
}

VoxelChunkWrapper::VoxelChunkWrapper(VoxelData initData)
{
    m_realChunk = new VoxelEmptyChunk(initData);
    m_tesselDirty = true;
}

void VoxelChunkWrapper::morphTick()
{
    m_realChunk = m_realChunk->morph();
}

void VoxelChunkWrapper::setVoxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData const& rhs)
{
    m_realChunk = m_realChunk->setVoxelData(x, y, z, rhs);
    m_tesselDirty = true;
}

bool VoxelChunkWrapper::isTesselDirty()
{
    return m_tesselDirty;
}

void VoxelChunkWrapper::clearTesselDirty()
{
    m_tesselDirty = false;
}

void VoxelChunkWrapper::voxelData(unsigned int x, unsigned int y, unsigned int z, VoxelData& out)
{
    m_realChunk->voxelData(x, y, z, out);
}


// DEBUG- Function

VoxelChunkWrapper* createRandomChunk()
{
    VoxelData myData;
    myData.vId = 0;
    VoxelChunkWrapper* newChunk = new VoxelChunkWrapper(myData);

    for (unsigned int x = 0; x < 32; ++x)
        for (unsigned int y = 0; y < 32; ++y) for (unsigned int z = 0; z < 32; ++z)
        {
            myData.vId = std::rand() % 64;
            newChunk->setVoxelData(x, y, z, myData);
        }

    return newChunk;
}