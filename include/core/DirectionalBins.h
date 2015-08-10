#ifndef _BATCH_H_
#define _BATCH_H_

#include <iostream>
#include <fstream>
#include <string>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <tbb/concurrent_queue.h>

#include <core/Common.h>
#include <core/RayCompressed.h>
#include <core/BatchItem.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Single bin to store compressed ray data
 * 
 * This stores the current memory mapped file as well as its path, size, writable position
 * and individual mutex.
 */
struct Bin
{
  boost::iostreams::mapped_file_sink outfile;
  std::string path;

  RayCompressed* pointer;
  RayCompressed* end;
  size_t size;

  boost::mutex mutex;
};

/**
 * @brief      A shared container for six cardinal bins of compressed rays
 * 
 * The DirectionalBins class combines six standard bins with one for each cardinal direction. It
 * also manages adding data from local buffers efficiently flushing this data to the batch queue
 * when required. If flushing the data is done without care to the order of which bins are cleared
 * first then performance will be dramatically reduced.
 */
class DirectionalBins
{
public:
  /**
   * @brief      This constructor will initialize each cardinal bin and open the mapped files
   *
   * @param[in]  _exponent  the bin size exponent
   */
  DirectionalBins(size_t _exponent);

  /**
   * @brief      This destructor will close any remaining mapped files and remove them from disk
   */
  ~DirectionalBins();

  /**
   * @brief      Add buffer of compressed rays to an indexed bin
   *
   * @param[in]  _size         size of array to be added
   * @param[in]  _cardinal     index of which directional bin
   * @param      _data         input ray data
   * @param      _batch_queue  output queue to store batch representation
   */
  void add(const int _size, const int _cardinal, RayCompressed* _data, tbb::concurrent_queue< BatchItem >* _batch_queue);

  /**
   * @brief      Flush data into batch queue for processing
   *
   * @param      _batch_queue  output queue to store batch representaton
   */
  void flush(tbb::concurrent_queue< BatchItem >* _batch_queue);

private:
  size_t m_exponent;
  Bin m_bin[6];
};

MSC_NAMESPACE_END

#endif
