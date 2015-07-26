#ifndef _PATHTRACER_H_
#define _PATHTRACER_H_

#include <string>

#include <boost/scoped_ptr.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/OpenImageWrapper.h>
#include <core/DirectionalBins.h>
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
  boost::scoped_ptr< DirectionalBins > m_bins;
  boost::scoped_ptr< Settings > m_settings;
  boost::scoped_ptr< Image > m_image;
  boost::scoped_ptr< Scene > m_scene;
  boost::scoped_ptr< CameraInterface > m_camera;
  boost::scoped_ptr< FilterInterface > m_filter;
  boost::scoped_ptr< SamplerInterface > m_sampler;

  LocalTextureSystem m_thread_texture_system;
  LocalRandomGenerator m_thread_random_generator;

  tbb::concurrent_queue< BatchItem > m_batch_queue;

  void construct(const std::string &_filename);
  void cameraSampling();
  void fileLoading(const BatchItem& batch_info, RayCompressed* batch_compressed);
  void rayDecompressing(const BatchItem& batch_info, RayCompressed* batch_compressed, RayUncompressed* batch_uncompressed);
  void raySorting(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void sceneTraversal(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void hitPointSorting(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void surfaceShading(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
};

MSC_NAMESPACE_END

#endif