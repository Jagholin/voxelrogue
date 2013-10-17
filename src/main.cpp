#include <osgDB\ReadFile>
#include <osg\Group>
#include <osg\ShapeDrawable>
#include <osgViewer\Viewer>
#include <chrono>

#include "voxelchunks.h"
#include <iostream>

int main()
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Group> root = new osg::Group;

    osg::ref_ptr<osg::Geode> cubeNode = new osg::Geode;
    osg::ref_ptr<osg::ShapeDrawable> cubeDrawable = new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 10));
    cubeDrawable->setColor(osg::Vec4(1,0,0,1));
    cubeNode->addDrawable(cubeDrawable);
    root->addChild(cubeNode);

    osg::ref_ptr <osg::Node> cessnaNode = osgDB::readNodeFile("cessna.osg");

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    osg::ref_ptr<VoxelChunkWrapper> myChunk = createRandomChunk();
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(endTime-startTime).count() << std::endl;

    viewer.setSceneData(cessnaNode.get());

    return viewer.run();
}