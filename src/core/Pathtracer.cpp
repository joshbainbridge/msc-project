#include <vector>

#include <yaml-cpp/yaml.h>

#include <core/Pathtracer.h>
#include <core/GeometricCamera.h>
#include <core/TentFilter.h>
#include <core/StratifiedSampler.h>

MSC_NAMESPACE_BEGIN

void Pathtracer::construct(const std::string &_filename)
{
  m_scene.reset(new Scene);
  m_bin.reset(new GlobalBin);

  std::vector<YAML::Node> node_root = YAML::LoadAllFromFile(_filename);

  YAML::Node node_setup = node_root[0];
  YAML::Node node_scene = node_root[1];

  {
    Image* image = new Image;

    image->width = 700;
    image->height = 500;
    image->base = 8;

    if(node_setup["image"])
      *image = node_setup["image"].as<Image>();

    size_t sample_count = image->width * image->height * image->base;
    image->samples.resize(sample_count);

    size_t pixel_count = image->width * image->height;
    image->pixels.resize(pixel_count);
    for(size_t i = 0; i < pixel_count; ++i)
    {
      image->pixels[i].r = 0.f;
      image->pixels[i].g = 0.f;
      image->pixels[i].b = 0.f;
    }

    m_image.reset(image);
  }

  {
    Settings* settings = new Settings;

    settings->ray_depth = 4;
    settings->bucket_size = 16;
    settings->bin_exponent = 11;
    settings->batch_exponent = 25;
    settings->iteration = 0;

    if(node_setup["settings"])
      *settings = node_setup["settings"].as<Settings>();

    m_settings.reset(settings);
  }

  {
    GeometricCamera* camera = new GeometricCamera();

    camera->origin(Vector3f(0.f, 0.f, 0.f));
    camera->direction(Vector3f(0.f, 0.f, 1.f));
    camera->focalLength(5.f);
    camera->focalDistance(100.f);
    camera->aperture(10.f);

    m_camera.reset(camera);

    if(node_setup["camera"])
    {
      if(node_setup["camera"]["type"].as< std::string >() == "Geometric")
      {
        GeometricCamera* geometric_camera = new GeometricCamera();
        *geometric_camera = node_setup["camera"].as<GeometricCamera>();
        m_camera.reset(geometric_camera);
      }
    }
  }

  {
    TentFilter* filter = new TentFilter();

    m_filter.reset(filter);

    if(node_setup["filter"])
    {
      if(node_setup["filter"]["type"].as< std::string >() == "Tent")
      {
        TentFilter* tent_filter = new TentFilter();
        *tent_filter = node_setup["filter"].as<TentFilter>();
        m_filter.reset(tent_filter);
      }
    }
  }

  {
    StratifiedSampler* sampler = new StratifiedSampler();

    m_sampler.reset(sampler);

    if(node_setup["sampler"])
    {
      if(node_setup["sampler"]["type"].as< std::string >() == "Stratified")
      {
        StratifiedSampler* stratified_sampler = new StratifiedSampler();
        *stratified_sampler = node_setup["sampler"].as<StratifiedSampler>();
        m_sampler.reset(stratified_sampler);
      }
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

Pathtracer::Pathtracer(const std::string &_filename)
{
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

  construct(_filename);
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
  *_pixels = &(m_image->pixels[0].v[0]);

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