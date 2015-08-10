#ifndef _LAYEREDTEXTURE_H_
#define _LAYEREDTEXTURE_H_

#include <core/Common.h>
#include <core/TextureInterface.h>
#include <core/StandardTexture.h>
#include <core/ConstantTexture.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the texture interface and represents a layered texturing system
 * 
 * This texture takes two base textures and combines them using a mask texture. Each texture and
 * the mask can be of any other texture type, that currently being a constant, standard texture or
 * possibly another texture. As there is no integral to be sampled, russian roulette is not used
 * as this would introduce variance with minimal performance increase.
 */
class LayeredTexture : public TextureInterface
{
public:
  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline TextureInterface upper() const {return *m_upper;}
  
  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline TextureInterface lower() const {return *m_lower;}

  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline TextureInterface mask() const {return *m_mask;}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void upper(TextureInterface* _upper){m_upper.reset(_upper);}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void lower(TextureInterface* _lower){m_lower.reset(_lower);}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void mask(TextureInterface* _mask){m_mask.reset(_mask);}

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

  /**
   * @brief      Gets colour value according to index
   *
   * @param[in]  _index  colour index to find value for specific position
   *
   * @return     colour according to index
   */
  Colour3f colour(const size_t _index) const;

private:
  boost::shared_ptr< TextureInterface > m_upper;
  boost::shared_ptr< TextureInterface > m_lower;
  boost::shared_ptr< TextureInterface > m_mask;
  std::vector< Colour3f > m_colour;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::LayeredTexture>
{
  static bool decode(const Node& node, msc::LayeredTexture& rhs)
  {
    if(node.size() != 4)
      return false;

    {
      if(node["upper"]["type"].as< std::string >() == "Standard")
      {
        msc::StandardTexture* standard_texture = new msc::StandardTexture();
        *standard_texture = node["upper"].as<msc::StandardTexture>();
        rhs.upper(standard_texture);
      }

      if(node["upper"]["type"].as< std::string >() == "Constant")
      {
        msc::ConstantTexture* constant_texture = new msc::ConstantTexture();
        *constant_texture = node["upper"].as<msc::ConstantTexture>();
        rhs.upper(constant_texture);
      }

      if(node["upper"]["type"].as< std::string >() == "Layered")
      {
        msc::LayeredTexture* layered_texture = new msc::LayeredTexture();
        *layered_texture = node["upper"].as<msc::LayeredTexture>();
        rhs.upper(layered_texture);
      }
    }

    {
      if(node["lower"]["type"].as< std::string >() == "Standard")
      {
        msc::StandardTexture* standard_texture = new msc::StandardTexture();
        *standard_texture = node["lower"].as<msc::StandardTexture>();
        rhs.lower(standard_texture);
      }

      if(node["lower"]["type"].as< std::string >() == "Constant")
      {
        msc::ConstantTexture* constant_texture = new msc::ConstantTexture();
        *constant_texture = node["lower"].as<msc::ConstantTexture>();
        rhs.lower(constant_texture);
      }

      if(node["lower"]["type"].as< std::string >() == "Layered")
      {
        msc::LayeredTexture* layered_texture = new msc::LayeredTexture();
        *layered_texture = node["lower"].as<msc::LayeredTexture>();
        rhs.lower(layered_texture);
      }
    }

    {
      if(node["mask"]["type"].as< std::string >() == "Standard")
      {
        msc::StandardTexture* standard_texture = new msc::StandardTexture();
        *standard_texture = node["mask"].as<msc::StandardTexture>();
        rhs.mask(standard_texture);
      }

      if(node["mask"]["type"].as< std::string >() == "Constant")
      {
        msc::ConstantTexture* constant_texture = new msc::ConstantTexture();
        *constant_texture = node["mask"].as<msc::ConstantTexture>();
        rhs.mask(constant_texture);
      }

      if(node["mask"]["type"].as< std::string >() == "Layered")
      {
        msc::LayeredTexture* layered_texture = new msc::LayeredTexture();
        *layered_texture = node["mask"].as<msc::LayeredTexture>();
        rhs.mask(layered_texture);
      }
    }

    return true;
  }
};

YAML_NAMESPACE_END

#endif