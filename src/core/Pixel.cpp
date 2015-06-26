#include <core/Pixel.h>

MSC_NAMESPACE_BEGIN

//Get pixel colour as average of samples
Colour3f Pixel::pixelColour() const
{
  int nsamples = m_samples.size();
  Colour3f summation(0.f, 0.f, 0.f);
  
  if (nsamples != 0)
  {
    for (int sample_i = 0; sample_i < nsamples; ++sample_i)
    {
      summation += m_samples[sample_i];
    }
  }
  else
  {
    nsamples = 1;
  }
  
  return summation / nsamples;
}

//Add colour sample to pixel
void Pixel::addSample(const Colour3f _sample)
{
  m_samples.push_back(_sample);
}

//Get a specific sample value
Colour3f Pixel::getSample(const int _index) const
{
  return m_samples[_index];
}

MSC_NAMESPACE_END