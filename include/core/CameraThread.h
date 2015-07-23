#ifndef _CAMERATHREAD_H_
#define _CAMERATHREAD_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/Buffer.h>
#include <core/Image.h>
#include <core/BatchItem.h>
#include <core/DirectionalBins.h>
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
  CameraThread(Buffer _buffer) : m_buffer(_buffer) {;}

  void start(
    DirectionalBins* _bin,
    CameraInterface* _camera,
    SamplerInterface* _sampler,
    Image* _image,
    tbb::concurrent_queue< CameraTask >* _camera_queue,
    tbb::concurrent_queue< BatchItem >* _batch_queue
    );

  void join();
  void process(
    DirectionalBins* _bin,
    CameraInterface* _camera,
    SamplerInterface* _sampler,
    Image* _image,
    tbb::concurrent_queue< CameraTask >* _camera_queue,
    tbb::concurrent_queue< BatchItem >* _batch_queue
    );

private:
  boost::thread m_thread;
  Buffer m_buffer;

  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif