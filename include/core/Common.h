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

template < typename type > bool compare(const type _a, const type _b)
{
  return fabs(static_cast<float>(_a) - static_cast<float>(_b)) < M_EPSILON;
}

template < typename type > void cartesianToSphericalCoord(const type _x, const type _y, const type _z, float *azimuthal, float *polar)
{
  *polar = atan(_y / _x);
  *azimuthal = atan(sqrt(_x * _x + _y * _y) / _z);
}

template < typename type > void sphericalToCartesianCoord(const type _azimuthal, const type _polar, float *x, float *y, float *z)
{
  *x = cos(_polar) * sin(_azimuthal);
  *y = sin(_polar) * sin(_azimuthal);
  *z = cos(_azimuthal);
}

template < typename type, int size > struct BoundingBox
{
  type min[size];
  type max[size];
};

typedef BoundingBox< float, 3 > BoundingBox3f;

// template < typename t_origin, typename t_direction > struct Ray
// {
//   t_origin origin;
//   t_direction dir;
// };

typedef Eigen::Matrix< float, 2, 1 > Vector2f;
typedef Eigen::Matrix< float, 3, 1 > Vector3f;
typedef Eigen::Matrix< float, 4, 1 > Vector4f;
typedef Eigen::Matrix< float, 4, 4 > TransMatrix;
// typedef Ray< Vector2f, Vector2f > Ray2f;
// typedef Ray< Vector3f, Vector3f > Ray3f;
// typedef Ray< Vector4f, Vector4f > Ray4f;
typedef Eigen::Array< float, 3, 1 > Colour3f;

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::Vector3f>
{
  static Node encode(const msc::Vector3f& rhs)
  {
    Node node;
    node.push_back(rhs.x());
    node.push_back(rhs.y());
    node.push_back(rhs.z());
    return node;
  }

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
  static Node encode(const msc::Vector2f& rhs)
  {
    Node node;
    node.push_back(rhs.x());
    node.push_back(rhs.y());
    return node;
  }

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
  static Node encode(const msc::Colour3f& rhs)
  {
    Node node;
    node.push_back(rhs[0]);
    node.push_back(rhs[1]);
    node.push_back(rhs[2]);
    return node;
  }

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