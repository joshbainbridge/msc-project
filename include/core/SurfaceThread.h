#ifndef _SURFACETHREAD_H_
#define _SURFACETHREAD_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/LocalBin.h>
#include <core/GlobalBin.h>
#include <core/Scene.h>
#include <core/Image.h>
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
    boost::shared_ptr< LocalBin > _local_bin,
    boost::shared_ptr< GlobalBin > _global_bin,
    boost::shared_ptr< Scene > _scene,
    boost::shared_ptr< Image > _image
    )
    : m_local_bin(_local_bin)
    , m_global_bin(_global_bin)
    , m_scene(_scene)
    , m_image(_image)
  {;}

  void start(tbb::concurrent_queue< SurfaceTask >* _queue);
  void join();
  void process(tbb::concurrent_queue< SurfaceTask >* _queue);

private:
  boost::thread m_thread;
  boost::shared_ptr< LocalBin > m_local_bin;
  boost::shared_ptr< GlobalBin > m_global_bin;
  boost::shared_ptr< Scene > m_scene;
  boost::shared_ptr< Image > m_image;

  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif