#ifndef _SURFACETHREAD_H_
#define _SURFACETHREAD_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>
#include <OpenImageIO/texture.h>

#include <core/Common.h>
#include <core/Bin.h>
#include <core/Batch.h>
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
    boost::shared_ptr< Bin > _local_bin,
    boost::shared_ptr< Batch > _batch,
    boost::shared_ptr< Scene > _scene,
    boost::shared_ptr< Image > _image
    )
    : m_local_bin(_local_bin)
    , m_batch(_batch)
    , m_scene(_scene)
    , m_image(_image)
  {;}

  void start(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _batch);
  void join();
  void process(tbb::concurrent_queue< SurfaceTask >* _queue, RayUncompressed* _batch);

private:
  boost::thread m_thread;
  boost::shared_ptr< Bin > m_local_bin;
  boost::shared_ptr< Batch > m_batch;
  boost::shared_ptr< Scene > m_scene;
  boost::shared_ptr< Image > m_image;

  OpenImageIO::TextureSystem* m_texture_system;

  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif