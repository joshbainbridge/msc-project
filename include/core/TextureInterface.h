#ifndef _TEXTUREINTERFACE_H_
#define _TEXTUREINTERFACE_H_

#include <vector>

#include <core/Common.h>
#include <core/OpenImageWrapper.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for surface shaders
 * 
 * This is an interface for using a texture in a polymorphic sense. It describes three main methods
 * that are responsible for cloning the class to allow for mutation on each thread locally,
 * initializing the texture to cache the texture colour and finally retrieving the value according
 * to an index.
 */
class TextureInterface
{
public:
  /**
   * @brief      Virtual destructor required for interface
   */
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