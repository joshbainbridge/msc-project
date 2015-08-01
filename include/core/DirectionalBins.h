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
 */
class DirectionalBins
{
public:
  DirectionalBins(size_t _exponent);
  ~DirectionalBins();

  /**
   * @brief      Add array of compressed rays to an indexed bin
   *
   * @param[in]  _size         size of array to be added
   * @param[in]  _cardinal     index of which directional bin
   * @param      _data         input ray data
   * @param      _batch_queue  output queue to store batch representation
   */
  void add(const int _size, const int _cardinal, RayCompressed* _data, tbb::concurrent_queue< BatchItem >* _batch_queue);
  void flush(tbb::concurrent_queue< BatchItem >* _batch_queue);

private:
  size_t m_exponent;
  Bin m_bin[6];
};

MSC_NAMESPACE_END

#endif
