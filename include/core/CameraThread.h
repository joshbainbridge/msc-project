#ifndef _CAMERATHREAD_H_
#define _CAMERATHREAD_H_

#include <boost/thread.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/LocalBin.h>
#include <core/GlobalBin.h>
#include <core/CameraInterface.h>
#include <core/SamplerInterface.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

struct CameraTask
{
  size_t begin_x;
  size_t begin_y;
  size_t end_x;
  size_t end_y;
};

class CameraThread
{ 
public:
  CameraThread(
    tbb::concurrent_queue< CameraTask > _task_queue,
    boost::shared_ptr< LocalBin > _local_bin,
    boost::weak_ptr< GlobalBin > _global_bin,
    boost::weak_ptr< CameraInterface > _camera,
    boost::weak_ptr< SamplerInterface > _sampler
    )
    : m_task_queue(_task_queue)
    , m_local_bin(_local_bin)
    , m_global_bin(_global_bin)
    , m_camera(_camera)
    , m_sampler(_sampler)
  {;}

  void start();
  void join();
  void process();

private:
  boost::thread m_thread;
  tbb::concurrent_queue< CameraTask > m_task_queue;
  boost::shared_ptr< LocalBin > m_local_bin;
  boost::weak_ptr< GlobalBin > m_global_bin;
  boost::weak_ptr< CameraInterface > m_camera;
  boost::weak_ptr< SamplerInterface > m_sampler;
  
  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif