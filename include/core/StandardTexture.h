#ifndef _STANDARDTEXTURE_H_
#define _STANDARDTEXTURE_H_

#include <core/Common.h>
#include <core/TextureInterface.h>
#include <core/Singleton.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Inherits from the shader interface and represents a non-cntributing surface
 * 
 * When initialized this will create a std::vector containing texture data for each position in a
 * range. Texture lookup is vectorized and sorted according to object and geometric primative. This
 * allows for optimal usage of memory when reading in large textures sets. 
 */
class StandardTexture : public TextureInterface
{
public:
  /**
   * @brief      Getter method for texture path
   *
   * @return     texture path as string
   */
  inline std::string string() const {return m_string.string();}

  /**
   * @brief      Setter method for texture path
   *
   * @param[in]  _colour  texture path as string
   */
  inline void string(const std::string _string){m_string = OpenImageIO::ustring(_string);}

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
  OpenImageIO::ustring m_string;
  std::vector< Colour3f > m_colour;
};

MSC_NAMESPACE_END

YAML_NAMESPACE_BEGIN

template<> struct convert<msc::StandardTexture>
{
  static bool decode(const Node& node, msc::StandardTexture& rhs)
  {
    if(node.size() != 2)
      return false;

    msc::SingletonString& scene_root = msc::SingletonString::instance();
    rhs.string(scene_root.getData().append("/").append(node["string"].as<std::string>()));

    return true;
  }
};

YAML_NAMESPACE_END

#endif