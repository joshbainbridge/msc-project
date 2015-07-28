#include <fstream>

#include <tbb/tbb.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#include <core/Pathtracer.h>
#include <core/EmbreeWrapper.h>
#include <core/ThinLensCamera.h>
#include <core/PinHoleCamera.h>
#include <core/TentFilter.h>
#include <core/BoxFilter.h>
#include <core/StratifiedSampler.h>
#include <core/IndependentSampler.h>
#include <core/GridSampler.h>
#include <core/PolygonObject.h>
#include <core/LambertShader.h>
#include <core/QuadLight.h>
#include <core/RaySort.h>
#include <core/RayIntersect.h>
#include <core/RayDecompress.h>
#include <core/RayBoundingbox.h>
#include <core/Convolve.h>
#include <core/Camera.h>
#include <core/Shading.h>

MSC_NAMESPACE_BEGIN

void Pathtracer::construct(const std::string &_filename)
{
  std::vector<YAML::Node> node_root = YAML::LoadAllFromFile(_filename);

  YAML::Node node_setup = node_root[0];
  YAML::Node node_scene = node_root[1];

  {
    Image* image = new Image;

    image->width = 700;
    image->height = 500;
    image->base = 8;
    image->iteration = 0;

    if(node_setup["image"])
      *image = node_setup["image"].as<Image>();

    size_t sample_count = image->width * image->height * image->base * image->base;
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
    settings->shading_size = 4096;
    settings->buffer_exponent = 11;
    settings->bin_exponent = 25;

    if(node_setup["settings"])
      *settings = node_setup["settings"].as<Settings>();

    m_settings.reset(settings);
  }

  {
    ThinLensCamera* camera = new ThinLensCamera();

    m_camera.reset(camera);

    if(node_setup["camera"])
    {
      if(node_setup["camera"]["type"].as< std::string >() == "ThinLens")
      {
        ThinLensCamera* thinlens_camera = new ThinLensCamera();
        *thinlens_camera = node_setup["camera"].as<ThinLensCamera>();
        m_camera.reset(thinlens_camera);
      }

      if(node_setup["camera"]["type"].as< std::string >() == "PinHole")
      {
        PinHoleCamera* pinhole_camera = new PinHoleCamera();
        *pinhole_camera = node_setup["camera"].as<PinHoleCamera>();
        m_camera.reset(pinhole_camera);
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

      if(node_setup["filter"]["type"].as< std::string >() == "Box")
      {
        BoxFilter* box_filter = new BoxFilter();
        *box_filter = node_setup["filter"].as<BoxFilter>();
        m_filter.reset(box_filter);
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

      if(node_setup["sampler"]["type"].as< std::string >() == "Independent")
      {
        IndependentSampler* independent_sampler = new IndependentSampler();
        *independent_sampler = node_setup["sampler"].as<IndependentSampler>();
        m_sampler.reset(independent_sampler);
      }

      if(node_setup["sampler"]["type"].as< std::string >() == "Grid")
      {
        GridSampler* grid_sampler = new GridSampler();
        *grid_sampler = node_setup["sampler"].as<GridSampler>();
        m_sampler.reset(grid_sampler);
      }
    }
  }

  m_scene.reset(new Scene);
  m_scene->rtc_scene = rtcNewScene(RTC_SCENE_STATIC | RTC_SCENE_COHERENT, RTC_INTERSECT1);
  
  for(YAML::const_iterator scene_iterator = node_scene.begin(); scene_iterator != node_scene.end(); ++scene_iterator)
  {
    YAML::Node first = scene_iterator->first;
    YAML::Node second = scene_iterator->second;

    if(first.as< std::string >() == "object")
    {
      if(second["type"].as< std::string >() == "Polygon")
      {
        boost::shared_ptr< PolygonObject > polygon_object(new PolygonObject);
        *polygon_object = second.as<PolygonObject>();

        size_t geom_id = rtcNewTriangleMesh(
          m_scene->rtc_scene,
          RTC_GEOMETRY_STATIC,
          polygon_object->indices.size() / 3,
          polygon_object->positions.size() / 4
          );

        rtcSetBuffer(
          m_scene->rtc_scene,
          geom_id,
          RTC_VERTEX_BUFFER,
          &(polygon_object->positions[0]),
          0,
          4 * sizeof(float)
          );

        rtcSetBuffer(
          m_scene->rtc_scene,
          geom_id,
          RTC_INDEX_BUFFER,
          &(polygon_object->indices[0]),
          0,
          3 * sizeof(unsigned int)
          );

        m_scene->objects.push_back(polygon_object);
      }
    }

    if(first.as< std::string >() == "shader")
    {
      if(second["type"].as< std::string >() == "Lambert")
      {
        boost::shared_ptr< LambertShader > lambert_shader(new LambertShader);
        *lambert_shader = second.as<LambertShader>();

        m_scene->shaders.push_back(lambert_shader);
      }
    }
  }
  
  for(YAML::const_iterator light_iterator = node_scene.begin(); light_iterator != node_scene.end(); ++light_iterator)
  {
    YAML::Node first = light_iterator->first;
    YAML::Node second = light_iterator->second;

    if(first.as< std::string >() == "light")
    {
      if(second["type"].as< std::string >() == "Quad")
      {
        boost::shared_ptr< QuadLight > quad_light(new QuadLight);
        *quad_light = second.as<QuadLight>();

        m_scene->lights.push_back(quad_light);
      }
    }
  }

  rtcCommit(m_scene->rtc_scene);
}

void Pathtracer::cameraSampling()
{
  // Create primary rays from camera
  tbb::parallel_for(
    tbb::blocked_range2d< size_t >(0, m_image->width, 32, 0, m_image->height, 32),
    Camera(m_camera.get(), m_sampler.get(), m_image.get(), m_bins.get(), &m_batch_queue, &m_thread_random_generator),
    tbb::simple_partitioner()
    );
}

bool Pathtracer::batchLoading(BatchItem* batch_info)
{
  // Query and load from batch queue 
  if(!m_batch_queue.try_pop(*batch_info))
  {
    m_bins->flush(&m_batch_queue);
    return m_batch_queue.try_pop(*batch_info);
  }

  return true;
}

void Pathtracer::fileLoading(const BatchItem& batch_info, RayCompressed* batch_compressed)
{
  // Load batch from file
  std::ifstream infile;
  infile.open(batch_info.filename);
  infile.read((char *)batch_compressed, sizeof(RayCompressed) * batch_info.size);
  infile.close();
}

void Pathtracer::rayDecompressing(const BatchItem& batch_info, RayCompressed* batch_compressed, RayUncompressed* batch_uncompressed)
{
  // Decompress rays
  tbb::parallel_for(tbb::blocked_range< size_t >(0, batch_info.size, 1024), RayDecompress(batch_compressed, batch_uncompressed));
}

void Pathtracer::raySorting(const BatchItem& batch_info, RayUncompressed* batch_uncompressed)
{
  // Find bounding box and sort rays
  RayBoundingbox limits(batch_uncompressed);
  tbb::parallel_reduce(tbb::blocked_range< size_t >(0, batch_info.size, 1024), limits);
  RaySort(0, batch_info.size, limits.value(), batch_uncompressed)();
}

void Pathtracer::sceneTraversal(const BatchItem& batch_info, RayUncompressed* batch_uncompressed)
{
  // Traverse scene with sorted rays
  tbb::parallel_for(tbb::blocked_range< size_t >(0, batch_info.size, 128), RayIntersect(m_scene.get(), batch_uncompressed));
}

void Pathtracer::hitPointSorting(const BatchItem& batch_info, RayUncompressed* batch_uncompressed)
{
  // Sort hit points according to geometry and primitives
  tbb::parallel_sort(&batch_uncompressed[0], &batch_uncompressed[batch_info.size], CompareHit());
}

void Pathtracer::surfaceShading(const BatchItem& batch_info, RayUncompressed* batch_uncompressed)
{
  // Intergrate shading and create secondary rays
  tbb::parallel_for(
    RangeGeom< RayUncompressed* >(0, batch_info.size, batch_uncompressed),
    Shading(m_scene.get(), m_image.get(), m_bins.get(), &m_batch_queue, &m_thread_texture_system, &m_thread_random_generator, batch_uncompressed),
    tbb::simple_partitioner()
    );
}

void Pathtracer::imageConvolution()
{
  // Convolve iamge using filter interface
  tbb::parallel_for(
    tbb::blocked_range2d< size_t >(0, m_image->width, 64, 0, m_image->height, 64),
    Convolve(m_filter.get(), m_image.get())
    );
}

Pathtracer::Pathtracer(const std::string &_filename)
{
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
  rtcInit(NULL);

  LocalTextureSystem m_thread_texture_system(nullTextureSystem);

  construct(_filename);
}

Pathtracer::~Pathtracer()
{
  for(LocalTextureSystem::const_iterator i = m_thread_texture_system.begin(); i != m_thread_texture_system.end();  ++i)
    OpenImageIO::TextureSystem::destroy(*i, false);

  rtcDeleteScene(m_scene->rtc_scene);
  rtcExit();
}

void Pathtracer::image(float** _pixels, int* _width, int* _height)
{
  *_pixels = &(m_image->pixels[0].v[0]);

  *_width = m_image->width;
  *_height = m_image->height;
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

  m_image->iteration = 0;
}

int Pathtracer::process()
{
  tbb::task_scheduler_init init(1);

  m_bins.reset(new DirectionalBins(m_settings->bin_exponent));

  size_t bin_size = pow(2, m_settings->bin_exponent);
  RayCompressed* batch_compressed = new RayCompressed[bin_size];
  RayUncompressed* batch_uncompressed = new RayUncompressed[bin_size];

  cameraSampling();

  std::cout << "\033[1;32mCurrent queue holds " << m_batch_queue.unsafe_size() << " batches.\033[0m" << std::endl;

  BatchItem pre_batch_info;
  bool pre_batch_found = batchLoading(&pre_batch_info);

  BatchItem post_batch_info;
  bool post_batch_found = batchLoading(&post_batch_info);

  if(pre_batch_found)
    fileLoading(pre_batch_info, batch_compressed);

  boost::thread loading_thread;

  while(pre_batch_found)
  {
    rayDecompressing(pre_batch_info, batch_compressed, batch_uncompressed);

    if(post_batch_found)
      loading_thread = boost::thread(&Pathtracer::fileLoading, this, post_batch_info, batch_compressed);

    raySorting(pre_batch_info, batch_uncompressed);

    sceneTraversal(pre_batch_info, batch_uncompressed);

    hitPointSorting(pre_batch_info, batch_uncompressed);

    surfaceShading(pre_batch_info, batch_uncompressed);

    if(post_batch_found)
      loading_thread.join();

    boost::filesystem::remove(pre_batch_info.filename);

    std::swap(pre_batch_found, post_batch_found);
    std::swap(pre_batch_info, post_batch_info);
    post_batch_found = batchLoading(&post_batch_info);
  }

  delete[] batch_uncompressed;
  delete[] batch_compressed;

  imageConvolution();

  m_image->iteration += 1;
  return m_image->iteration;
}

MSC_NAMESPACE_END