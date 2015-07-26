#ifndef _LAMBERTSHADER_H_
#define _LAMBERTSHADER_H_

#include <string>

#include <core/Common.h>
#include <core/ShaderInterface.h>

MSC_NAMESPACE_BEGIN

class LambertShader : public ShaderInterface
{
public:
  void evaluate(
    const Vector3f _input,
    const Vector3f _output,
    const Vector3f _normal,
    TextureSystem _texture_system,
    const float _u,
    const float _v,
    Colour3f* _weight
    ) const;

  inline Colour3f colour() const {return m_colour;}
  inline std::string texture() const {return m_texture.string();}

  inline void colour(const Colour3f _colour){m_colour = _colour;}
  inline void texture(const std::string _texture){m_texture = OpenImageIO::ustring(_texture);}

private:
  Colour3f m_colour;
  OpenImageIO::ustring m_texture;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::LambertShader>
{
  static Node encode(const msc::LambertShader& rhs)
  {
    Node node;
    node["shader"]["type"] = "Lambert";
    node["shader"]["colour"] = rhs.colour();
    node["shader"]["texture"] = rhs.texture();
    return node;
  }

  static bool decode(const Node& node, msc::LambertShader& rhs)
  {
    if(!node.IsMap() || node.size() != 3)
      return false;

    rhs.colour(node["colour"].as<msc::Colour3f>());
    rhs.texture(node["texture"].as<std::string>());
    return true;
  }
};

YAML_NAMESPACE_END

#endif