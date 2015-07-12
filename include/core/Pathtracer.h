#ifndef _PATHTRACER_H_
#define _PATHTRACER_H_

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/EmbreeWrapper.h>
#include <core/GlobalBin.h>
#include <core/CameraThread.h>
#include <core/SurfaceThread.h>
#include <core/Settings.h>
#include <core/Image.h>
#include <core/Scene.h>
#include <core/CameraInterface.h>
#include <core/FilterInterface.h>
#include <core/SamplerInterface.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class Pathtracer
{
public:
  Pathtracer(const std::string &_filename);

  void image(float** _pixels, int* _with, int* _height);
  void clear();

  int process();

private:
  boost::shared_ptr< GlobalBin > m_bin;
  boost::shared_ptr< Settings > m_settings;
  boost::shared_ptr< Image > m_image;
  boost::shared_ptr< Scene > m_scene;
  boost::shared_ptr< CameraInterface > m_camera;
  boost::shared_ptr< FilterInterface > m_filter;
  boost::shared_ptr< SamplerInterface > m_sampler;

  size_t m_nthreads;
  std::vector< boost::shared_ptr< CameraThread > > m_camera_threads;
  std::vector< boost::shared_ptr< SurfaceThread > > m_surface_threads;

  tbb::concurrent_queue< CameraTask > m_camera_queue;
  tbb::concurrent_queue< SurfaceTask > m_surface_queue;
  tbb::concurrent_queue< std::string > m_batch_queue;
  
  RandomGenerator m_random;

  void construct(const std::string &_filename);
  void createThreads();
  void createCameraTasks();
  void createSurfaceTasks();
  void runCameraThreads();
};

MSC_NAMESPACE_END

#endif