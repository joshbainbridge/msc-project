#include <core/LayeredTexture.h>

MSC_NAMESPACE_BEGIN

TextureInterface* LayeredTexture::clone()
{
  LayeredTexture* texture = new LayeredTexture(*this);
  texture->upper(m_upper->clone());
  texture->lower(m_lower->clone());
  texture->mask(m_mask->clone());
  return texture;
}

void LayeredTexture::initialize(
  const size_t _size,
  std::vector< float >& _u,
  std::vector< float >& _v,
  TextureSystem _texture_system
  )
{
  m_upper->initialize(_size, _u, _v, _texture_system);
  m_lower->initialize(_size, _u, _v, _texture_system);
  m_mask->initialize(_size, _u, _v, _texture_system);

  m_colour.resize(_size);
  for(size_t index = 0; index < _size; ++index)
  {
    Colour3f mask = m_mask->colour(index);
    Colour3f inverse = Colour3f(1.f, 1.f, 1.f) - mask;
    m_colour[index] = (m_upper->colour(index) * mask) + (m_lower->colour(index) * inverse);
  }
}

Colour3f LayeredTexture::colour(const size_t _index) const
{
  return m_colour[_index];
}

MSC_NAMESPACE_END