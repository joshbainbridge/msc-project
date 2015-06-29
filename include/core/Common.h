#ifndef _COMMON_H_
#define _COMMON_H_

#include <stddef.h>
#include <math.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <core/Constants.h>

MSC_NAMESPACE_BEGIN

template < typename type > bool compare(const type _a, const type _b)
{
  return fabs(static_cast<float>(_a) - static_cast<float>(_b)) < M_EPSILON;
}

template < typename type > void cartesianToSphericalCoord(const type _x, const type _y, const type _z, float *azimuthal, float *polar)
{
  float x = static_cast<float>(_x);
  float y = static_cast<float>(_y);
  float z = static_cast<float>(_z);
  *polar = atan(y / x);
  *azimuthal = atan(sqrt(x * x + y * y) / z);
}

template < typename type > void sphericalToCartesianCoord(const type _azimuthal, const type _polar, float *x, float *y, float *z)
{
  *x = cos(_polar) * sin(_azimuthal);
  *y = sin(_polar) * sin(_azimuthal);
  *z = cos(_azimuthal);
}

template < typename t_origin, typename t_direction > struct Ray
{
  t_origin origin;
  t_direction dir;
};

typedef Eigen::Matrix< float, 2, 1 > Vector2f;
typedef Eigen::Matrix< float, 3, 1 > Vector3f;
typedef Eigen::Matrix< float, 4, 1 > Vector4f;
typedef Eigen::Matrix< float, 4, 4 > TransMatrix;
typedef Ray< Vector2f, Vector2f > Ray2f;
typedef Ray< Vector3f, Vector3f > Ray3f;
typedef Ray< Vector4f, Vector4f > Ray4f;
typedef Eigen::Array< float, 3, 1 > Colour3f;

MSC_NAMESPACE_END

#endif