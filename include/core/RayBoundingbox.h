#ifndef _RAYBOUNDINGBOX_H_
#define _RAYBOUNDINGBOX_H_

#include <tbb/tbb.h>

#include <core/Common.h>
#include <core/RayUncompressed.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Functor class to find bounding box from an array of rays
 */
class RayBoundingbox
{
public:
  RayBoundingbox(RayUncompressed* _data)
   : m_data(_data)
  {;}

  RayBoundingbox(RayBoundingbox& s, tbb::split )
   : m_data(s.m_data)
  {;}

  /**
   * @brief      Operator overloader to allow the class to act as a functor with tbb
   * 
   * @param[in]  r           a one dimensional range over an array of rays
   */
  void operator()(const tbb::blocked_range< size_t >& r);

  /**
   * @brief      Join method to allow for a parallel reduction algorithm and avoid thread contention
   *
   * @param      rhs   { parameter_description }
   */
  void join( RayBoundingbox& rhs );

  /**
   * @brief      Getter method for bounding box
   *
   * @return     bounding box
   */
  inline BoundingBox3f value() const {return m_value;}

private:
  RayUncompressed* m_data;
  BoundingBox3f m_value;
};

MSC_NAMESPACE_END

#endif