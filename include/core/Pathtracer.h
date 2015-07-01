#ifndef _PATHTRACER_H_
#define _PATHTRACER_H_

#include <string>

#include <boost/scoped_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/GlobalBin.h>
#include <core/Settings.h>
#include <core/CameraInterface.h>
#include <core/Image.h>
#include <core/Scene.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

struct CameraTask
{
  size_t begin_x;
  size_t begin_y;
  size_t end_x;
  size_t end_y;
};

struct SurfaceTask
{
  size_t begin;
  size_t end;
};

class Pathtracer
{
public:
  Pathtracer(const std::string &_filename);

  void clear();
  void image(float** _pixels, int* _with, int* _height);
  int process();

private:
  boost::scoped_ptr< GlobalBin > m_bin;
  boost::scoped_ptr< Settings > m_settings;
  boost::scoped_ptr< CameraInterface > m_camera;
  boost::scoped_ptr< Image > m_image;
  boost::scoped_ptr< Scene > m_scene;

  tbb::concurrent_queue< CameraTask > m_camera_queue;
  tbb::concurrent_queue< SurfaceTask > m_surface_queue;
  tbb::concurrent_queue< std::string > m_batch_queue;
  
  RandomGenerator m_random;
};

MSC_NAMESPACE_END

#endif