#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/OpenImageWrapper.h>
#include <core/Buffer.h>
#include <core/DirectionalBins.h>
#include <core/Scene.h>
#include <core/Image.h>
#include <core/BatchItem.h>
#include <core/RayUncompressed.h>
#include <core/RayCompressed.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      A range template that describes a one dimensional range containing an equal comparison
 *
 * @tparam     type  Type of data in range that must be comparible
 */
template < typename type > class RangeGeom {
public:
  RangeGeom(size_t _begin, size_t _end, size_t _grainsize, type _array)
   : m_begin(_begin)
   , m_end(_end)
   , m_grainsize(_grainsize)
   , m_array(_array)
  {;}

  /**
   * @brief      Splitting function to allow construction of equal value ranges with tbb
   *
   * @param      r          previously constructed range
   * @param[in]  <unnamed>  required to adhere to tbb's specification
   */
  RangeGeom(RangeGeom &r, tbb::split)
  {
    m_array = r.m_array;
    m_grainsize = r.m_grainsize;
    m_end = r.m_end;

    size_t division = r.m_begin;
    for(size_t index = r.m_begin; index < r.m_end; ++index)
    {
      if(m_array[r.m_begin] != m_array[index])
      {
        division = index;
        break;
      }
      else if((index - r.m_begin) == m_grainsize)
      {
        division = index;
        break;
      }
    }

    m_begin = division;
    r.m_end = division;
  }

  /**
   * @brief      Check if range is empty
   *
   * @return     boolean value
   */
  bool empty() const {return m_begin == m_end;}

  /**
   * @brief      Is range currently divisible into sub-ranges
   *
   * @return     boolean value
   */
  bool is_divisible() const
  {
    return (m_array[m_begin] != m_array[m_end - 1]) || ((m_end - m_begin) > m_grainsize);
  }

  /**
   * @brief      Getter method for first index of range
   *
   * @return     first index
   */
  size_t begin() const {return m_begin;}

  /**
   * @brief      Getter method for first index in range
   *
   * @return     last index
   */
  size_t end() const {return m_end;}

  static const bool is_splittable_in_proportion = false;

public:
  size_t m_begin;
  size_t m_end;
  size_t m_grainsize;
  type m_array;
};

/**
 * @brief      Lighting intergrator for surface intersections
 */
class Integrator
{
public:
  Integrator(
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

  /**
   * @brief      Operator overloader to allow the class to act as a functor with tbb
   * 
   * @param[in]  r           a constant range according to geometry id over found hit point
   */
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