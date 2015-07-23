#ifndef _SURFACETHREAD_H_
#define _SURFACETHREAD_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>
#include <OpenImageIO/texture.h>

#include <core/Common.h>
#include <core/Buffer.h>
#include <core/DirectionalBins.h>
#include <core/Scene.h>
#include <core/Image.h>
#include <core/BatchItem.h>
#include <core/RayUncompressed.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

struct SurfaceTask
{
  size_t begin;
  size_t end;
};

class SurfaceThread
{ 
public:
  SurfaceThread(Buffer _buffer) : m_buffer(_buffer) {;}

  void start(
    DirectionalBins* _bin,
    Scene* _scene,
    Image* _image,
    tbb::concurrent_queue< SurfaceTask >* _surface_queue,
    tbb::concurrent_queue< BatchItem >* _batch_queue,
    RayUncompressed* _batch
    );

  void join();
  void process(
    DirectionalBins* _bin,
    Scene* _scene,
    Image* _image,
    tbb::concurrent_queue< SurfaceTask >* _surface_queue,
    tbb::concurrent_queue< BatchItem >* _batch_queue,
    RayUncompressed* _batch
    );

private:
  boost::thread m_thread;
  Buffer m_buffer;

  OpenImageIO::TextureSystem* m_texture_system;
  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif