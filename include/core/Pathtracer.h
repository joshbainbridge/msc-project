#ifndef _PATHTRACER_H_
#define _PATHTRACER_H_

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <OpenImageIO/texture.h>

#include <core/Common.h>
#include <core/DirectionalBins.h>
#include <core/CameraThread.h>
#include <core/SurfaceThread.h>
#include <core/Settings.h>
#include <core/Image.h>
#include <core/Scene.h>
#include <core/CameraInterface.h>
#include <core/FilterInterface.h>
#include <core/SamplerInterface.h>
#include <core/RayUncompressed.h>
#include <core/RayCompressed.h>
#include <core/RandomGenerator.h>
#include <core/BatchItem.h>

MSC_NAMESPACE_BEGIN

class Pathtracer
{
public:
  Pathtracer(const std::string &_filename);
  ~Pathtracer();

  void image(float** _pixels, int* _with, int* _height);
  void clear();

  int process();

private:
  // Change these back to scoped pointers
  boost::shared_ptr< DirectionalBins > m_bins;
  boost::shared_ptr< Settings > m_settings;
  boost::shared_ptr< Image > m_image;
  boost::shared_ptr< Scene > m_scene;
  boost::shared_ptr< CameraInterface > m_camera;
  boost::shared_ptr< FilterInterface > m_filter;
  boost::shared_ptr< SamplerInterface > m_sampler;

  // Should be removed
  size_t m_nthreads;
  std::vector< boost::shared_ptr< CameraThread > > m_camera_threads;
  std::vector< boost::shared_ptr< SurfaceThread > > m_surface_threads;

  tbb::concurrent_queue< BatchItem > m_batch_queue;
  tbb::concurrent_queue< CameraTask > m_camera_queue;
  tbb::concurrent_queue< SurfaceTask > m_surface_queue;

  OpenImageIO::TextureSystem* m_texture_system;

  void construct(const std::string &_filename);
  void createThreads();
  void cameraBegin();
  void surfaceBegin(size_t _size, RayUncompressed* _batch);
};

MSC_NAMESPACE_END

#endif