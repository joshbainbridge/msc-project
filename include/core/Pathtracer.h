#ifndef _PATHTRACER_H_
#define _PATHTRACER_H_

#include <string>

#include <boost/scoped_ptr.hpp>
#include <boost/atomic.hpp>
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

/**
 * @brief      Rendering engine and interface to external programs
 * 
 * Central class of system exposing main access to external application. It implements each part
 * of the paper in a series of parallel and concurrent operations. Batch processing is done for
 * example while loading new data into a buffer from disk to avoid thread downtime. The system
 * can also render multiple images and combine the results iteratively for fast feedback or produce
 * images more efficiently using larger sample counts. Processing can also be queried and terminated
 * externally through the public methods.
 */
class Pathtracer
{
public:
  /**
   * @brief      This constructor will construct the scene and initialise libraries
   *
   * @param[in]  _filename  file path to the scene description file
   */
  Pathtracer(const std::string &_filename);

  /**
   * @brief      The destructor will delete any files currently on the batch queue and close the cache
   */
  ~Pathtracer();

  /**
   * @brief      Retrieves internal data on the final image
   *
   * @param      _pixels  a pointer to internal pixel data
   * @param      _with    will be set to width of image
   * @param      _height  will be set to height of image
   */
  void image(float** _pixels, int* _with, int* _height);

  /**
   * @brief      Clear internal image data including samples and pixels
   */
  void clear();

  /**
   * @brief      Check if pathtracer is set to terminate
   */
  bool active();

  /**
   * @brief      Sets pathtracer to terminate iteration
   */
  void terminate();

  /**
   * @brief      Compute a single iteration of image
   *
   * @return     current itteration count
   */
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

  boost::atomic< bool > m_terminate;

  void construct(const std::string &_filename);
  void cameraSampling();
  bool batchLoading(BatchItem* batch_info);
  void fileLoading(const BatchItem& batch_info, RayCompressed* batch_compressed);
  void rayDecompressing(const BatchItem& batch_info, RayCompressed* batch_compressed, RayUncompressed* batch_uncompressed);
  void raySorting(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void sceneTraversal(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void hitPointSorting(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void surfaceShading(const BatchItem& batch_info, RayUncompressed* batch_uncompressed);
  void imageConvolution();
};

MSC_NAMESPACE_END

#endif