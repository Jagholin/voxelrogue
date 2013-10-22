#include <cassert>
#include "voxelnodes.h"

ChunkGraphNode::ChunkGraphNode(osg::Vec3i chunkCoord, osg::ref_ptr<VoxelChunkWrapper> const& chunk)
{
    m_chunkCoord = chunkCoord;
    if (chunk)
        m_chunk = chunk;
    else
        m_chunk = new VoxelChunkWrapper(VoxelData(0));
}

void ChunkGraphNode::loadChunkIn(osg::Vec3i chunkCoord, Directions at)
{

}

struct TesselatorUpdateCallback: public osg::Drawable::UpdateCallback
{
    virtual void update( osg::NodeVisitor* nv, osg::Drawable* dr)
    {
        //throw std::exception("The method or operation is not implemented.");
        VoxelTerrainChunkColumn* realDrawable = dynamic_cast<VoxelTerrainChunkColumn*>(dr);
        if (!realDrawable)
            return;

        for (osg::ref_ptr<VoxelChunkWrapper> myChunk: realDrawable->m_chunks)
        {
            if (myChunk->isTesselDirty())
                realDrawable->tesselateChunk(myChunk);
            realDrawable->dirtyDisplayList();
        }
    }
};

VoxelTerrainChunkColumn::VoxelTerrainChunkColumn()
{
    setUpdateCallback(new TesselatorUpdateCallback);
}

VoxelTerrainChunkColumn::VoxelTerrainChunkColumn(const VoxelTerrainChunkColumn& rhs, const osg::CopyOp& op)
{

}

osg::BoundingBox VoxelTerrainChunkColumn::computeBound() const
{
    //throw std::exception("The method or operation is not implemented.");

    return osg::BoundingBox(0, 0, 0, 3.2, 3.2, 3.2*m_chunks.size());
}

void VoxelTerrainChunkColumn::drawImplementation( osg::RenderInfo& renderInfo ) const
{
    //throw std::exception("The method or operation is not implemented.");

    osg::Camera *camera = renderInfo.getCurrentCamera();
    osg::Vec3f eye, viewTarget, up;
    camera->getViewMatrixAsLookAt(eye, viewTarget, up);

    osg::Matrix viewMatrix = camera->getViewMatrix();
    osg::Matrix modelViewMatrix = renderInfo.getState()->getModelViewMatrix();

    osg::Matrix modelMatrixInv = osg::Matrix::inverse(modelViewMatrix) * viewMatrix;
    // Convert eye coordinates to model space
    eye = modelMatrixInv * eye;
}

void VoxelTerrainChunkColumn::tesselateChunk(osg::ref_ptr<VoxelChunkWrapper> const& chunk)
{

}

