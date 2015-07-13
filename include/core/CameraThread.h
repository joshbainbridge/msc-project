#ifndef _CAMERATHREAD_H_
#define _CAMERATHREAD_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/Bin.h>
#include <core/Batch.h>
#include <core/CameraInterface.h>
#include <core/SamplerInterface.h>
#include <core/Image.h>
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
    boost::shared_ptr< Bin > _local_bin,
    boost::shared_ptr< Batch > _batch,
    boost::shared_ptr< CameraInterface > _camera,
    boost::shared_ptr< SamplerInterface > _sampler,
    boost::shared_ptr< Image > _image
    )
    : m_local_bin(_local_bin)
    , m_batch(_batch)
    , m_camera(_camera)
    , m_sampler(_sampler)
    , m_image(_image)
  {;}

  void start(tbb::concurrent_queue< CameraTask >* _queue);
  void join();
  void process(tbb::concurrent_queue< CameraTask >* _queue);

private:
  boost::thread m_thread;
  boost::shared_ptr< Bin > m_local_bin;
  boost::shared_ptr< Batch > m_batch;
  boost::shared_ptr< CameraInterface > m_camera;
  boost::shared_ptr< SamplerInterface > m_sampler;
  boost::shared_ptr< Image > m_image;

  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif