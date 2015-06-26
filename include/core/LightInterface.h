#ifndef _LIGHTINTERFACE_H_
#define _LIGHTINTERFACE_H_

#include <core/Common.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class LightInterface
{
public:
  LightInterface()
    : m_colour(Colour3f(1.f, 1.f, 1.f))
    , m_origin(Vector3f(0.f, 0.f, 0.f))
    , m_dir(Vector3f(0.f, 0.f, 1.f))
    , m_up(Vector3f(0.f, 1.f, 0.f))
    , m_right(Vector3f(1.f, 0.f, 0.f))
    , m_width(1.f)
    , m_height(1.f)
    , m_sine_exponent(1.f)
    , m_watts(1000.f)
  {;}

  virtual ~LightInterface() {}

  virtual Vector3f sampleLight(RandomGenerator &_random_generator) const =0;
  virtual Colour3f illumination(const Vector3f &light_direction) const =0;

  inline Vector3f origin() const {return m_origin;}
  inline Vector3f direction() const {return m_dir;}
  inline Vector3f up() const {return m_up;}
  inline Vector3f right() const {return m_right;}
  inline float width() const {return m_width;}
  inline float height() const {return m_height;}
  inline float exponent() const {return m_sine_exponent;}
  inline float watts() const {return m_watts;}
  inline Colour3f colour() const {return m_colour;}

  inline void origin(const Vector3f &_origin){m_origin = _origin;}
  inline void width(const float &_width){m_width = _width;}
  inline void height(const float &_height){m_height = _height;}
  inline void exponent(const float &_sine_exponent){m_sine_exponent = _sine_exponent;}
  inline void watts(const float &_watts){m_watts = _watts;}
  inline void colour(const Colour3f &_colour){m_colour = _colour;}

  void direction(const Vector3f &_dir);

protected:
  Colour3f m_colour;
  Vector3f m_origin;
  Vector3f m_dir;
  Vector3f m_up;
  Vector3f m_right;
  float m_width;
  float m_height;
  float m_sine_exponent;
  float m_watts;
};

MSC_NAMESPACE_END

#endif
