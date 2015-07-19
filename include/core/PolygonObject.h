#ifndef _POLYGONOBJECT_H_
#define _POLYGONOBJECT_H_

#include <string>
#include <vector>
#include <iostream>

#include <tinyobjloader/tiny_obj_loader.h>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

// Required so that data is aligned to 16 bytes
struct Vertex
{
  float x, y, z;
  float align;
};

struct PolygonObject
{
  std::string filename;
  Vector3f translation;
  Vector3f rotation;
  Vector3f scale;

  std::vector< Vertex > positions;
  std::vector< float > normals;
  std::vector< float > texcoords;
  std::vector< unsigned int > indices;

  size_t material_id;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::PolygonObject>
{
  static Node encode(const msc::PolygonObject& rhs)
  {
    Node node;
    node["object"]["filename"] = rhs.filename;
    node["object"]["translation"] = rhs.translation;
    node["object"]["rotation"] = rhs.rotation;
    node["object"]["scale"] = rhs.scale;
    node["object"]["material"] = rhs.material_id;
    return node;
  }

  static bool decode(const Node& node, msc::PolygonObject& rhs)
  {
    if(!node.IsMap() || node.size() != 6)
      return false;

    rhs.filename = node["filename"].as<std::string>();
    rhs.translation = node["translation"].as<msc::Vector3f>();
    rhs.rotation = node["rotation"].as<msc::Vector3f>();
    rhs.scale = node["scale"].as<msc::Vector3f>();
    rhs.material_id = node["material"].as<int>();

    tinyobj::shape_t shape;
    std::string err = tinyobj::LoadObj(shape, rhs.filename.c_str());

    if(!err.empty())
      std::cout << err << std::endl;

    size_t position_count = shape.mesh.positions.size() / 3;
    rhs.positions.resize(position_count);
    for(size_t i = 0; i < position_count; ++i)
    {
      rhs.positions[i].x = shape.mesh.positions[3 * i + 0];
      rhs.positions[i].y = shape.mesh.positions[3 * i + 1];
      rhs.positions[i].z = shape.mesh.positions[3 * i + 2];
    }
    rhs.normals.swap(shape.mesh.normals);
    rhs.texcoords.swap(shape.mesh.texcoords);
    rhs.indices.swap(shape.mesh.indices);

    std::cout << shape.mesh.positions.size() << ", " << rhs.normals.size() << ", " << rhs.texcoords.size() << ", " << rhs.indices.size() << std::endl;

    msc::Affine3f transform(msc::Affine3f::Identity());
    transform *= Eigen::Translation3f(rhs.translation);
    transform *= Eigen::AngleAxisf(rhs.rotation.x() * M_PI_180, msc::Vector3f(1, 0, 0));
    transform *= Eigen::AngleAxisf(rhs.rotation.y() * M_PI_180, msc::Vector3f(0, 1, 0));
    transform *= Eigen::AngleAxisf(rhs.rotation.z() * M_PI_180, msc::Vector3f(0, 0, 1));
    transform *= Eigen::AlignedScaling3f(rhs.scale);

    msc::Vector3fMap mapped_data(NULL);
    for(size_t i = 0; i < position_count; ++i)
    {
      new (&mapped_data) msc::Vector3fMap((float*) &(rhs.positions[i]));
      mapped_data = transform * mapped_data;
    }

    return true;
  }
};

YAML_NAMESPACE_END

#endif
