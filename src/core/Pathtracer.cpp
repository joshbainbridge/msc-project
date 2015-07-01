#include <vector>

#include <yaml-cpp/yaml.h>

#include <core/Pathtracer.h>
#include <core/GeometricCamera.h>

MSC_NAMESPACE_BEGIN

Pathtracer::Pathtracer(const std::string &_filename)
{
  std::vector<YAML::Node> node_root = YAML::LoadAllFromFile(_filename);

  YAML::Node node_setup = node_root[0];
  YAML::Node node_scene = node_root[1];

  {
    m_image.reset(new Image);

    int width = 700;
    int height = 500;
    int base = 8;

    if(node_setup["image"])
    {
      YAML::Node node_image = node_setup["image"];
      width = node_image["width"].as<int>();
      height = node_image["height"].as<int>();
      base = node_image["sample base"].as<int>();
    }

    size_t sample_count = width * height * base;
    m_image->samples.resize(sample_count);

    size_t pixel_count = width * height;
    m_image->pixels.resize(pixel_count);
    for(size_t i = 0; i < pixel_count; ++i)
    {
      m_image->pixels[i].r = 0.f;
      m_image->pixels[i].g = 0.f;
      m_image->pixels[i].b = 0.f;
    }

    m_image->width = width;
    m_image->height = height;
    m_image->base = base;
  }

  {
    m_settings.reset(new Settings);

    int ray_depth = 4;
    int bucket_size = 16;
    int iteration = 0;

    if(node_setup["settings"])
    {
      YAML::Node node_settings = node_setup["settings"];
      ray_depth = node_settings["ray depth"].as<int>();
      bucket_size = node_settings["bucket size"].as<int>();
    }

    m_settings->ray_depth = ray_depth;
    m_settings->bucket_size = bucket_size;
    m_settings->iteration = iteration;
  }

  // if(node_setup["camera"])
  // {
  //   YAML::Node node_camera = node_setup["camera"];
  //   int ray_depth = node_camera["ray depth"].as<int>();
  //   int bucket_size = node_camera["bucket size"].as<int>();

  //   m_settings.reset(new Settings(ray_depth, bucket_size));
  // }
  // else
  // {
  //   m_camera.reset(new GeometricCamera());
  // }
  
  // // Construct Scene
  // for(YAML::const_iterator setup_element = node_setup.begin(); setup_element != node_setup.end(); ++setup_element)
  // {
  //   std::cout << setup_element->first.as<std::string>() << std::endl;

  //   YAML::Node node_element = setup_element->second;
  //   for(YAML::const_iterator setup_value = node_element.begin(); setup_value != node_element.end(); ++setup_value)
  //   {
  //     if(setup_value->second.IsScalar())
  //       std::cout << "  " << setup_value->first.as<std::string>() << ": " << setup_value->second.as<std::string>() << std::endl;
  //   }
  // }

  m_scene.reset(new Scene);
  m_bin.reset(new GlobalBin);
}

void Pathtracer::clear()
{
  size_t pixel_count = m_image->width * m_image->height;
  for(size_t i = 0; i < pixel_count; ++i)
  {
    m_image->pixels[i].r = 0.f;
    m_image->pixels[i].g = 0.f;
    m_image->pixels[i].b = 0.f;
  }

  m_settings->iteration = 0;
}

void Pathtracer::image(float** _pixels, int* _width, int* _height)
{
  *_pixels = &(m_image->pixels[0].r);

  *_width = m_image->width;
  *_height = m_image->height;
}

int Pathtracer::process()
{
  size_t pixel_count = m_image->width * m_image->height;
  for(size_t i = 0; i < pixel_count; ++i)
  {
    m_image->pixels[i].r += m_random.getSample();
    m_image->pixels[i].g += m_random.getSample();
    m_image->pixels[i].b += m_random.getSample();
  }

  m_settings->iteration += 1;

  return m_settings->iteration;
}

MSC_NAMESPACE_END