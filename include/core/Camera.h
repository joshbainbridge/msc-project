#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <tbb/concurrent_queue.h>
#include <tbb/blocked_range2d.h>

#include <core/Common.h>
#include <core/Buffer.h>
#include <core/Image.h>
#include <core/BatchItem.h>
#include <core/DirectionalBins.h>
#include <core/CameraInterface.h>
#include <core/SamplerInterface.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class Camera
{
public:
  Camera(
    CameraInterface* _camera,
    SamplerInterface* _sampler,
    Image* _image,
    DirectionalBins* _bins,
    tbb::concurrent_queue< BatchItem >* _batch_queue,
    LocalRandomGenerator* _local_thread_storage
    )
   : m_camera(_camera)
   , m_sampler(_sampler)
   , m_image(_image)
   , m_bins(_bins)
   , m_batch_queue(_batch_queue)
   , m_local_thread_storage(_local_thread_storage)
  {;}

  void operator()(const tbb::blocked_range2d< size_t > &r) const;

private:
  CameraInterface* m_camera;
  SamplerInterface* m_sampler;
  Image* m_image;
  DirectionalBins* m_bins;
  tbb::concurrent_queue< BatchItem >* m_batch_queue;
  LocalRandomGenerator* m_local_thread_storage;

  mutable Buffer m_buffer;
};

MSC_NAMESPACE_END

#endif