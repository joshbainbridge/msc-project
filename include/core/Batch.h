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

MSC_NAMESPACE_BEGIN

class Batch
{
public:
  ~Batch();

  void construct(const int _exponent);
  void add(const int _size, const int _index, RayCompressed* _data);
  bool pop(std::string* batch);
  void clear();

private:
  size_t m_size;

  std::string m_paths[6];
  boost::iostreams::mapped_file_sink m_outfile[6];
  RayCompressed* m_pointers[6];
  RayCompressed* m_ends[6];
  boost::mutex m_mutexes[6];

  tbb::concurrent_queue< std::string > m_batch_queue;
};

MSC_NAMESPACE_END

#endif
