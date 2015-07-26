#include <core/Convolve.h>

MSC_NAMESPACE_BEGIN

void Convolve::operator()(const tbb::blocked_range2d< size_t > &r) const
{
  m_filter->convolve(
    m_image->width,
    m_image->height,
    m_image->base * m_image->base,
    r.rows().begin(),
    r.rows().end(),
    r.cols().begin(),
    r.cols().end(),
    &(m_image->samples[0]),
    &(m_image->pixels[0])
    );
}

MSC_NAMESPACE_END