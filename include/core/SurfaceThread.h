#ifndef _SURFACETHREAD_H_
#define _SURFACETHREAD_H_

#include <boost/thread.hpp>
#include <boost/weak_ptr.hpp>
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
    tbb::concurrent_queue< SurfaceTask > _task_queue,
    boost::shared_ptr< LocalBin > _local_bin,
    boost::weak_ptr< GlobalBin > _global_bin,
    boost::weak_ptr< Scene > _scene,
    boost::weak_ptr< Image > _image
    )
    : m_task_queue(_task_queue)
    , m_local_bin(_local_bin)
    , m_global_bin(_global_bin)
    , m_scene(_scene)
    , m_image(_image)
  {;}

  void start();
  void join();
  void process();

private:
  boost::thread m_thread;
  tbb::concurrent_queue< SurfaceTask > m_task_queue;
  boost::shared_ptr< LocalBin > m_local_bin;
  boost::weak_ptr< GlobalBin > m_global_bin;
  boost::weak_ptr< Scene > m_scene;
  boost::weak_ptr< Image > m_image;
  
  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif