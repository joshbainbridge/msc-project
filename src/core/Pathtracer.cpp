#include <vector>

#include <yaml-cpp/yaml.h>

#include <core/Pathtracer.h>
#include <core/GeometricCamera.h>
#include <core/TentFilter.h>
#include <core/StratifiedSampler.h>

MSC_NAMESPACE_BEGIN

Pathtracer::Pathtracer(const std::string &_filename)
{
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

  m_scene.reset(new Scene);
  m_bin.reset(new GlobalBin);

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
    int batch_exponent = 25;
    int iteration = 0;

    if(node_setup["settings"])
    {
      YAML::Node node_settings = node_setup["settings"];

      ray_depth = node_settings["ray depth"].as<int>();
      bucket_size = node_settings["bucket size"].as<int>();
      batch_exponent = node_settings["batch exponent"].as<int>();
    }

    m_settings->ray_depth = ray_depth;
    m_settings->bucket_size = bucket_size;
    m_settings->batch_exponent = batch_exponent;
    m_settings->iteration = iteration;
  }

  {
    m_camera.reset(new GeometricCamera());

    if(node_setup["camera"])
    {
      YAML::Node node_camera = node_setup["camera"];

      if(node_camera["type"].as< std::string >() == "Geometric")
      {
        GeometricCamera* geometric_camera = new GeometricCamera();

        float focal_distance = node_camera["focal distance"].as<float>();
        float aperture = node_camera["aperture"].as<float>();

        geometric_camera->focalDistance(focal_distance);
        geometric_camera->aperture(aperture);

        m_camera.reset(geometric_camera);
      }

      std::vector<float> origin = node_camera["origin"].as< std::vector<float> >();
      std::vector<float> direction = node_camera["origin"].as< std::vector<float> >();
      float focal_length = node_camera["focal length"].as<float>();

      m_camera->origin(Vector3f(origin[0], origin[1], origin[2]));
      m_camera->direction(Vector3f(direction[0], direction[1], direction[2]));
      m_camera->focalLength(focal_length);
    }

    // This needs to me moved to an argument input
    m_camera->ratio(static_cast<float>(m_image->width) / static_cast<float>(m_image->height));
  }

  {
    // Default setup
    m_filter.reset(new TentFilter());

    if(node_setup["filter"])
    {
      YAML::Node node_filter = node_setup["filter"];

      if(node_filter["type"].as< std::string >() == "Tent")
      {
        TentFilter* tent_filter = new TentFilter();

        // Type specific setup

        m_filter.reset(tent_filter);
      }

      // Interface specific setup
    }
  }

  {
    // Default setup
    m_sampler.reset(new StratifiedSampler());

    if(node_setup["sampler"])
    {
      YAML::Node node_sampler = node_setup["sampler"];

      if(node_sampler["type"].as< std::string >() == "Stratified")
      {
        StratifiedSampler* stratified_sampler = new StratifiedSampler();

        // Type specific setup

        m_sampler.reset(stratified_sampler);
      }

      // Interface specific setup
    }
  }
  
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