#ifndef _TEXTUREINTERFACE_H_
#define _TEXTUREINTERFACE_H_

#include <vector>

#include <core/Common.h>
#include <core/OpenImageWrapper.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for surface shaders
 */
class TextureInterface
{
public:
  virtual ~TextureInterface() {}

  /**
   * @brief      Create polymorphic copy of derived class
   *
   * @return     Is safe to have a covariant return type here
   */
  virtual TextureInterface* clone() = 0;

  /**
   * @brief      Initialize colour values potentially as vectorized texture lookup
   */
  virtual void initialize(
    const size_t _size,
    std::vector< float >& _u,
    std::vector< float >& _v,
    TextureSystem _texture_system
    ) =0;

  virtual Colour3f colour(const size_t _index) const =0;
};

MSC_NAMESPACE_END

#endif