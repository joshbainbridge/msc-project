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

struct Bin
{
  boost::iostreams::mapped_file_sink outfile;
  std::string path;

  RayCompressed* pointer;
  RayCompressed* end;
  size_t size;

  boost::mutex mutex;
};

class DirectionalBins
{
public:
  DirectionalBins(size_t _exponent);
  ~DirectionalBins();

  void add(const int _size, const int _cardinal, RayCompressed* _data, tbb::concurrent_queue< BatchItem >* _batch_queue);
  void flush(tbb::concurrent_queue< BatchItem >* _batch_queue);

private:
  size_t m_exponent;
  Bin m_bin[6];
};

MSC_NAMESPACE_END

#endif
