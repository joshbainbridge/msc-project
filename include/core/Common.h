#ifndef _COMMON_H_
#define _COMMON_H_

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <algorithm>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <yaml-cpp/yaml.h>

#include <core/Constants.h>

MSC_NAMESPACE_BEGIN

inline bool compare(const float _a, const float _b)
{
  return fabs(static_cast<float>(_a) - static_cast<float>(_b)) < M_EPSILON;
}

inline void cartesianToSphericalCoord(const float _x, const float _y, const float _z, float *azimuthal, float *zenith)
{
  *azimuthal = atan2(_y, _x);
  *zenith = atan(sqrt((_x * _x) + (_y * _y)) / _z);
}

inline void sphericalToCartesianCoord(const float _azimuthal, const float _zenith, float *x, float *y, float *z)
{
  *x = cos(_azimuthal) * sin(_zenith);
  *y = sin(_azimuthal) * sin(_zenith);
  *z = cos(_zenith);
}

inline float areaToAngleProbability(const float _area, const float _distance, const float _cosine)
{
  return _area * (_distance * _distance) / abs(_cosine);
}

inline float angleToAreaProbability(const float _angle, const float _distance, const float _cosine)
{
  return _angle * abs(_cosine) / (_distance * _distance);
}

inline float misBalance(const float _first, const float _second)
{
  return (_first / (_first + _second));
}

template < typename type, int size > struct BoundingBox
{
  type min[size];
  type max[size];
};

typedef BoundingBox< float, 3 > BoundingBox3f;

typedef Eigen::Matrix< float, 2, 1 > Vector2f;
typedef Eigen::Map< msc::Vector2f > Vector2fMap;
typedef Eigen::Matrix< float, 3, 1 > Vector3f;
typedef Eigen::Map< msc::Vector3f > Vector3fMap;
typedef Eigen::Matrix< float, 4, 1 > Vector4f;
typedef Eigen::Map< msc::Vector4f > Vector4fMap;
typedef Eigen::Array< float, 3, 1 > Colour3f;
typedef Eigen::Map< msc::Colour3f > Colour3fMap;

typedef Eigen::Transform< float, 3, Eigen::Affine > Affine3f;

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Vector3f>
{
  static bool decode(const Node& node, msc::Vector3f& rhs)
  {
    if(!node.IsSequence() || node.size() != 3)
    {
      return false;
    }

    rhs.x() = node[0].as<float>();
    rhs.y() = node[1].as<float>();
    rhs.z() = node[2].as<float>();
    return true;
  }
};

template<> struct convert<msc::Vector2f>
{
  static bool decode(const Node& node, msc::Vector2f& rhs)
  {
    if(!node.IsSequence() || node.size() != 2)
    {
      return false;
    }

    rhs.x() = node[0].as<float>();
    rhs.y() = node[1].as<float>();
    return true;
  }
};

template<> struct convert<msc::Colour3f>
{
  static bool decode(const Node& node, msc::Colour3f& rhs)
  {
    if(!node.IsSequence() || node.size() != 3)
    {
      return false;
    }

    rhs[0] = node[0].as<float>();
    rhs[1] = node[1].as<float>();
    rhs[2] = node[2].as<float>();
    return true;
  }
};

YAML_NAMESPACE_END

#endif