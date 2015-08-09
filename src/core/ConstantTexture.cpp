#include <core/ConstantTexture.h>
#include <vector>

MSC_NAMESPACE_BEGIN

TextureInterface* ConstantTexture::clone()
{
  return new ConstantTexture(*this);
}

void ConstantTexture::initialize(
  const size_t _size,
  std::vector< float >& _u,
  std::vector< float >& _v,
  TextureSystem _texture_system
  )
{
  // Nothing to initialize
}

Colour3f ConstantTexture::colour(const size_t _index) const
{
  return m_constant;
}

MSC_NAMESPACE_END