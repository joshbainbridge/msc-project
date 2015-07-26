#ifndef _SHADING_H_
#define _SHADING_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/OpenImageWrapper.h>
#include <core/Buffer.h>
#include <core/DirectionalBins.h>
#include <core/Scene.h>
#include <core/Image.h>
#include <core/BatchItem.h>
#include <core/RayUncompressed.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

template < typename type > class RangeGeom {
public:
  RangeGeom(size_t _begin, size_t _end, type _array)
   : m_begin(_begin)
   , m_end(_end)
   , m_array(_array)
  {;}

  RangeGeom(RangeGeom &r, tbb::split)
  {
    m_array = r.m_array;
    m_end = r.m_end;

    size_t division = r.m_begin;
    for(size_t iterator = r.m_begin; iterator < r.m_end; ++iterator)
    {
      if(m_array[r.m_begin] != m_array[iterator])
      {
        division = iterator;
        break;
      }
      else if((iterator - r.m_begin) == 4096)
      {
        division = iterator;
        break;
      }
    }

    m_begin = division;
    r.m_end = division;
  }

  bool empty() const {return m_begin == m_end;}
  bool is_divisible() const
  {
    return (m_array[m_begin] != m_array[m_end - 1]) || ((m_end - m_begin) > 4096);
  }

  size_t begin() const {return m_begin;}
  size_t end() const {return m_end;}

  static const bool is_splittable_in_proportion = false;

public:
  size_t m_begin;
  size_t m_end;
  type m_array;
};

class Shading
{
public:
  Shading(
    Scene* _scene,
    Image* _image,
    DirectionalBins* _bins,
    tbb::concurrent_queue< BatchItem >* _batch_queue,
    LocalTextureSystem* _local_thread_storage_texture,
    LocalRandomGenerator* _local_thread_storage_random,
    RayUncompressed* _batch
    )
   : m_scene(_scene)
   , m_image(_image)
   , m_batch(_batch)
   , m_bins(_bins)
   , m_batch_queue(_batch_queue)
   , m_local_thread_storage_texture(_local_thread_storage_texture)
   , m_local_thread_storage_random(_local_thread_storage_random)
  {;}

  void operator()(const RangeGeom< RayUncompressed* > &r) const;

private:
  Scene* m_scene;
  Image* m_image;
  RayUncompressed* m_batch;

  DirectionalBins* m_bins;
  tbb::concurrent_queue< BatchItem >* m_batch_queue;
  LocalTextureSystem* m_local_thread_storage_texture;
  LocalRandomGenerator* m_local_thread_storage_random;

  mutable Buffer m_buffer;
};

MSC_NAMESPACE_END

#endif