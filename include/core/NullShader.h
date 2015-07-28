#ifndef _NULLSHADER_H_
#define _NULLSHADER_H_

#include <core/Common.h>
#include <core/ShaderInterface.h>

MSC_NAMESPACE_BEGIN

class NullShader : public ShaderInterface
{
public:
  void evaluate(
    const int _size,
    TextureSystem _texture_system,
    const Vector3f* _input,
    const Vector3f* _output,
    const Vector3f* _normal,
    float* _u,
    float* _v,
    float* _result
    ) const;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::NullShader>
{
  static Node encode(const msc::NullShader& rhs)
  {
    Node node;
    node["shader"]["type"] = "Null";
    return node;
  }

  static bool decode(const Node& node, msc::NullShader& rhs)
  {
    if(!node.IsMap() || node.size() != 3)
      return false;

    return true;
  }
};

YAML_NAMESPACE_END

#endif