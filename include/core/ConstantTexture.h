#ifndef _ConstantTexture_H_
#define _ConstantTexture_H_

#include <core/Common.h>
#include <core/TextureInterface.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the shader interface and represents a non-cntributing surface
 */
class ConstantTexture : public TextureInterface
{
public:
  ConstantTexture()
    : m_constant(Colour3f(0.f, 0.f, 0.f))
  {;}

  /**
   * @brief      Getter method for colour coefficient
   *
   * @return     colour coefficient
   */
  inline Colour3f constant() const {return m_constant;}

  /**
   * @brief      Setter method for colour coefficient
   *
   * @param[in]  _colour  colour coefficient
   */
  inline void constant(const Colour3f _constant){m_constant = _constant;}

  /**
   * @brief      Create polymorphic copy of derived class
   *
   * @return     Is safe to have a covariant return type here
   */
  TextureInterface* clone();

  /**
   * @brief      Initialize colour values potentially as vectorized texture lookup
   */
  void initialize(
    const size_t _size,
    std::vector< float >& _u,
    std::vector< float >& _v,
    TextureSystem _texture_system
    );
  
  Colour3f colour(const size_t _index) const;

private:
  Colour3f m_constant;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::ConstantTexture>
{
  static bool decode(const Node& node, msc::ConstantTexture& rhs)
  {
    if(node.size() != 2)
      return false;

    rhs.constant(node["colour"].as<msc::Colour3f>());

    return true;
  }
};

YAML_NAMESPACE_END

#endif