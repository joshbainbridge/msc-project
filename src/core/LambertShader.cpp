#include <core/LambertShader.h>
#include <vector>

MSC_NAMESPACE_BEGIN

void LambertShader::evaluate(
  const int _size,
  TextureSystem _texture_system,
  const Vector3f* _input,
  const Vector3f* _output,
  const Vector3f* _normal,
  float* _u,
  float* _v,
  float* _result
  ) const
{
  OpenImageIO::TextureOptions options;
  std::vector< OpenImageIO::Runflag > runflags(_size, 1);
  std::vector< float > pattern(_size * 3);
  
  float nullvalue = 0;
  _texture_system->texture(
    m_texture,
    options,
    &(runflags[0]),
    0, _size,
    OpenImageIO::Varying(_u), OpenImageIO::Varying(_v),
    OpenImageIO::Uniform(nullvalue), OpenImageIO::Uniform(nullvalue),
    OpenImageIO::Uniform(nullvalue), OpenImageIO::Uniform(nullvalue),
    3, &(pattern[0])
    );

  for(size_t i = 0; i < _size; ++i)
  {
    _result[3 * i + 0] = m_colour[0] * pattern[3 * i + 0];
    _result[3 * i + 1] = m_colour[1] * pattern[3 * i + 1];
    _result[3 * i + 2] = m_colour[2] * pattern[3 * i + 2];
  }
}

MSC_NAMESPACE_END