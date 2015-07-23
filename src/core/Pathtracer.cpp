#include <fstream>

#include <tbb/tbb.h>
#include <boost/thread.hpp>

#include <core/Pathtracer.h>
#include <core/EmbreeWrapper.h>
#include <core/Buffer.h>
#include <core/GeometricCamera.h>
#include <core/TentFilter.h>
#include <core/StratifiedSampler.h>
#include <core/PolygonObject.h>
#include <core/LambertShader.h>
#include <core/RaySort.h>
#include <core/RayIntersect.h>
#include <core/RayDecompress.h>
#include <core/RayBoundingbox.h>

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
    GeometricCamera* camera = new GeometricCamera();

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

  m_scene.reset(new Scene);
  m_scene->rtc_scene = rtcNewScene(RTC_SCENE_STATIC, RTC_INTERSECT1);
  
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
        // PolygonObject* polygon_object = new PolygonObject();
        // *polygon_object = second.as<PolygonObject>();
        // m_scene.add(polygon_object);
      }
    }
  }

  rtcCommit(m_scene->rtc_scene);
}

void Pathtracer::createThreads()
{
  m_nthreads = boost::thread::hardware_concurrency();

  if(m_nthreads > 1)
    std::cout << "\033[1;32m" << m_nthreads << " supported threads found.\033[0m" << std::endl;

  if(!(m_nthreads > 1))
    std::cout << "\033[1;32mSingle thread found, system will function as a serial operation.\033[0m" << std::endl;

  Buffer buffer;
  buffer.size = pow(2, m_settings->buffer_exponent);
  for(size_t iterator_buffer = 0; iterator_buffer < 6; ++iterator_buffer)
  {
    buffer.direction[iterator_buffer].resize(buffer.size);
    buffer.index[iterator_buffer] = 0;
  }

  for(size_t i = 0; i < m_nthreads; ++i)
  {
    m_camera_threads.push_back(boost::shared_ptr< CameraThread >(new CameraThread(buffer)));
    m_surface_threads.push_back(boost::shared_ptr< SurfaceThread >(new SurfaceThread(buffer)));
  }
}

void Pathtracer::cameraBegin()
{
  size_t bucket_size = m_settings->bucket_size;
  size_t task_count_x = ceil(m_image->width / static_cast<float>(bucket_size));
  size_t task_count_y = ceil(m_image->height / static_cast<float>(bucket_size));

  for(size_t i = 0; i < task_count_x; ++i)
  {
    for(size_t j = 0; j < task_count_y; ++j)
    {
      //Check for any pixels left after resolution division with bucket size
      size_t task_width = bucket_size;
      if((i * bucket_size + bucket_size) > m_image->width)
        task_width = m_image->width - (i * bucket_size);

      //Check for any pixels left after resolution division with bucket size
      size_t task_height = bucket_size;
      if((j * bucket_size + bucket_size) > m_image->height)
        task_height = m_image->height - (j * bucket_size);

      CameraTask task;
      task.begin_x = i * bucket_size;
      task.begin_y = j * bucket_size;
      task.end_x = task.begin_x + task_width;
      task.end_y = task.begin_y + task_height;

      m_camera_queue.push(task);
    }
  }

  for(int iterator = 0; iterator < m_nthreads; ++iterator)
    m_camera_threads[iterator]->start(
      m_bins.get(),
      m_camera.get(),
      m_sampler.get(),
      m_image.get(),
      &m_camera_queue,
      &m_batch_queue
      );

  for(int iterator = 0; iterator < m_nthreads; ++iterator)
    m_camera_threads[iterator]->join();
}

void Pathtracer::surfaceBegin(size_t _size, RayUncompressed* _batch)
{
  int current_id = _batch[0].geomID;
  size_t current_index = 0;
  for(size_t iterator = 0; iterator < _size; ++iterator)
  {
    if(current_id != _batch[iterator].geomID)
    {
      SurfaceTask task;
      task.begin = current_index;
      task.end = iterator;

      m_surface_queue.push(task);

      current_id = _batch[iterator].geomID;
      current_index = iterator;
    }
    else if((iterator - current_index) > 4096)
    {
      SurfaceTask task;
      task.begin = current_index;
      task.end = iterator;

      m_surface_queue.push(task);

      current_index = iterator;
    }
  }

  SurfaceTask task;
  task.begin = current_index;
  task.end = _size;

  m_surface_queue.push(task);

  for(int iterator = 0; iterator < m_nthreads; ++iterator)
    m_surface_threads[iterator]->start(
      m_bins.get(),
      m_scene.get(),
      m_image.get(),
      &m_surface_queue,
      &m_batch_queue,
      _batch
      );

  for(int iterator = 0; iterator < m_nthreads; ++iterator)
    m_surface_threads[iterator]->join();
}

Pathtracer::Pathtracer(const std::string &_filename)
{
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

  rtcInit(NULL);

  m_texture_system = OpenImageIO::TextureSystem::create(true);

  construct(_filename);
  createThreads();
}

Pathtracer::~Pathtracer()
{
  OpenImageIO::TextureSystem::destroy(m_texture_system, true);

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

static inline bool operator<(const RayUncompressed &lhs, const RayUncompressed &rhs)
{
  return (lhs.geomID < rhs.geomID) || (lhs.geomID == rhs.geomID && lhs.primID < rhs.primID);
}

int Pathtracer::process()
{
  m_bins.reset(new DirectionalBins(m_settings->bin_exponent));
  
  size_t bin_size = pow(2, m_settings->bin_exponent);
  RayCompressed* batch_compressed = new RayCompressed[bin_size];
  RayUncompressed* batch_uncompressed = new RayUncompressed[bin_size];

  cameraBegin(); // replace with tbb (primary rays)
  m_bins->flush(&m_batch_queue);
  
  BatchItem batch_info;
  while(m_batch_queue.try_pop(batch_info))
  {
    // Move into seperate function
    std::ifstream infile;
    infile.open(batch_info.filename);
    infile.read((char *)batch_compressed, sizeof(RayCompressed) * batch_info.size);
    infile.close();

    tbb::parallel_for(tbb::blocked_range< size_t >(0, batch_info.size, 1024), RayDecompress(batch_compressed, batch_uncompressed));

    RayBoundingbox limits(batch_uncompressed);
    tbb::parallel_reduce(tbb::blocked_range< size_t >(0, batch_info.size, 1024), limits);

    RaySort &task = *new(tbb::task::allocate_root()) RaySort(0, batch_info.size, limits.value(), batch_uncompressed);
    tbb::task::spawn_root_and_wait(task);

    tbb::parallel_for(tbb::blocked_range< size_t >(0, batch_info.size, 128), RayIntersect(&(*m_scene), batch_uncompressed));

    // replace with tbb (secondary rays)
    tbb::parallel_sort(&batch_uncompressed[0], &batch_uncompressed[batch_info.size]);
    surfaceBegin(batch_info.size, batch_uncompressed);

    // m_bins->flush(&m_batch_queue);

    boost::filesystem::remove(batch_info.filename);
  }

  delete[] batch_uncompressed;
  delete[] batch_compressed;

  size_t sample_count = m_image->base * m_image->base;
  for(size_t iterator_x = 0; iterator_x < m_image->width; ++iterator_x)
  {
    for(size_t iterator_y = 0; iterator_y < m_image->height; ++iterator_y)
    {
      float total_r = 0.f;
      float total_g = 0.f;
      float total_b = 0.f;

      for(size_t iterator = 0; iterator < sample_count; ++iterator)
      {
        size_t sample_index = (iterator_x * m_image->height * sample_count) + (iterator_y * sample_count) + (iterator);
        total_r += m_image->samples[sample_index].r;
        total_g += m_image->samples[sample_index].g;
        total_b += m_image->samples[sample_index].b;
      }

      size_t pixel_index = iterator_y * m_image->width + iterator_x;
      m_image->pixels[pixel_index].r += (total_r / sample_count);
      m_image->pixels[pixel_index].g += (total_g / sample_count);
      m_image->pixels[pixel_index].b += (total_b / sample_count);
    }
  }

  m_image->iteration += 1;
  return m_image->iteration;
}

MSC_NAMESPACE_END