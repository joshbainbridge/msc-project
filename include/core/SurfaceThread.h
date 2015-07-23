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
  SurfaceThread(
    boost::shared_ptr< Buffer > _buffer,
    boost::shared_ptr< DirectionalBins > _bin,
    boost::shared_ptr< Scene > _scene,
    boost::shared_ptr< Image > _image
    )
    : m_buffer(_buffer)
    , m_bin(_bin)
    , m_scene(_scene)
    , m_image(_image)
  {;}

  void start(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _bin);
  void join();
  void process(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _bin);

private:
  boost::thread m_thread;
  boost::shared_ptr< Buffer > m_buffer;
  boost::shared_ptr< DirectionalBins > m_bin;
  boost::shared_ptr< Scene > m_scene;
  boost::shared_ptr< Image > m_image;

  OpenImageIO::TextureSystem* m_texture_system;

  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif