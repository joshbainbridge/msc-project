#include <core/StandardTexture.h>
#include <vector>

MSC_NAMESPACE_BEGIN

TextureInterface* StandardTexture::clone()
{
  return new StandardTexture(*this);
}

void StandardTexture::initialize(
  const size_t _size,
  std::vector< float >& _u,
  std::vector< float >& _v,
  TextureSystem _texture_system
  )
{
  OpenImageIO::TextureOptions options;
  options.swrap = OpenImageIO::TextureOptions::WrapPeriodic;
  options.twrap = OpenImageIO::TextureOptions::WrapPeriodic;
  std::vector< OpenImageIO::Runflag > runflags(_size, 1);
  std::vector< float > temp_colour(_size * 3);
  
  float nullvalue = 0;
  _texture_system->texture(
    m_string,
    options,
    &(runflags[0]),
    0, _size,
    OpenImageIO::Varying(&(_u[0])), OpenImageIO::Varying(&(_v[0])),
    OpenImageIO::Uniform(nullvalue), OpenImageIO::Uniform(nullvalue),
    OpenImageIO::Uniform(nullvalue), OpenImageIO::Uniform(nullvalue),
    3, &(temp_colour[0])
    );

  m_colour.resize(_size);
  Colour3fMap mapped_data(NULL);
  for(size_t index = 0; index < _size; ++index)
  {
    new (&mapped_data) Colour3fMap((float*) &(temp_colour[3 * index + 0]));
    m_colour[index] = mapped_data;
  }
}

Colour3f StandardTexture::colour(const size_t _index) const
{
  return m_colour[_index];
}

MSC_NAMESPACE_END