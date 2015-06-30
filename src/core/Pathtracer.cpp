#include <vector>

#include <yaml-cpp/yaml.h>

#include <core/Pathtracer.h>

#define WIDTH 700
#define HEIGHT 500

MSC_NAMESPACE_BEGIN

Pathtracer::Pathtracer(const std::string &_filename)
{
  std::vector<YAML::Node> node_root = YAML::LoadAllFromFile(_filename);

  YAML::Node node_setup = node_root[0];
  YAML::Node node_scene = node_root[1];
  
  // std::cout << "Setup size is " << node_setup.size() << std::endl;
  // std::cout << "Scene size is " << node_scene.size() << std::endl;
  
  // std::cout << std::endl;
  
  // std::cout << "Setup nodes are:" << std::endl;
  // for(YAML::const_iterator it = node_setup.begin(); it != node_setup.end(); ++it)
  // {
  //   std::cout << it->first.as<std::string>() << std::endl;
  //   if(it->first.as<std::string>() == "settings")
  //   {
  //     std::cout << "  " << it->second["ray depth"] << std::endl;
  //   }
  // }
  
  // std::cout << std::endl;
  
  // std::cout << "Scene nodes are:" << std::endl;
  // for(YAML::const_iterator it = node_scene.begin(); it != node_scene.end(); ++it)
  // {
  //   std::cout << it->first.as<std::string>() << std::endl;
  // }

  m_image.reset(new Image(WIDTH, HEIGHT, 8));
}

void Pathtracer::clear()
{
  // Clear image and reset iteration
}

void Pathtracer::image(float** _pixels, int* _width, int* _height)
{
  *_pixels = &(m_image->pixels[0]);

  *_width = WIDTH;
  *_height = HEIGHT;
}

MSC_NAMESPACE_END