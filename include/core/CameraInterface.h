#ifndef _CAMERAINTERFACE_H_
#define _CAMERAINTERFACE_H_

#include <core/Common.h>
#include <core/RayCompressed.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for render camera
 * 
 * This is a simple interface for using a camera in a polymorphic sense. As this operation is
 * unlikely to be used on single pixels it takes a range of data to compute the final result.
 */
class CameraInterface
{
public:
  /**
   * @brief      Virtual destructor required for interface 
   */
  virtual ~CameraInterface() {}

  /**
   * @brief      Creates primary rays from camera
   *
   * @param[in]  _count      sample count to process
   * @param      _positions  positions on film plane of samples
   * @param      _random     thread local random generator to prevent mutation
   * @param      _ouput      output of compressed rays
   */
  virtual void sample(const int _count, float* _positions, RandomGenerator* _random, RayCompressed* _ouput) const =0;
};

MSC_NAMESPACE_END

#endif
