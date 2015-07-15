#include <fstream>

#include <boost/thread.hpp>

#include <core/Pathtracer.h>
#include <core/Bin.h>
#include <core/GeometricCamera.h>
#include <core/TentFilter.h>
#include <core/StratifiedSampler.h>
#include <core/RayCompressed.h>
#include <core/RayUncompressed.h>
#include <core/RaySort.h>
#include <core/RayIntersect.h>

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
    settings->bin_exponent = 11;
    settings->batch_exponent = 25;

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

  m_batch.reset(new Batch);

  m_scene.reset(new Scene);
  m_scene->rtc_scene = rtcNewScene(RTC_SCENE_COHERENT, RTC_INTERSECT4);
  
  for(YAML::const_iterator scene_iterator = node_scene.begin(); scene_iterator != node_scene.end(); ++scene_iterator)
  {
    YAML::Node first = scene_iterator->first;
    YAML::Node second = scene_iterator->second;

    if(first.as< std::string >() == "object")
    {
      if(second["type"].as< std::string >() == "Polygon")
      {
        // PolygonObject* polygon_object = new PolygonObject();
        // *polygon_object = second.as<PolygonObject>();
        // m_scene.add(polygon_object);
      }
    }

    if(first.as< std::string >() == "light")
    {
      if(second["type"].as< std::string >() == "Quad")
      {
        // PolygonObject* polygon_object = new PolygonObject();
        // *polygon_object = second.as<PolygonObject>();
        // m_scene.add(polygon_object);
      }
    }

    if(first.as< std::string >() == "material")
    {
      if(second["type"].as< std::string >() == "Lambert")
      {
        // PolygonObject* polygon_object = new PolygonObject();
        // *polygon_object = second.as<PolygonObject>();
        // m_scene.add(polygon_object);
      }
    }
  }
}

void Pathtracer::createThreads()
{
  m_nthreads = boost::thread::hardware_concurrency();
//  m_nthreads = 1;

  if(m_nthreads > 1)
    std::cout << m_nthreads << " supported threads found" << std::endl;

  if(!(m_nthreads > 1))
    std::cout << "Single thread found, system will function as a serial operation" << std::endl;

  for(size_t i = 0; i < m_nthreads; ++i)
  {
    boost::shared_ptr< Bin > local_bin(new Bin());
    local_bin->size = pow(2, m_settings->bin_exponent);
    for(size_t iterator_bin = 0; iterator_bin < 6; ++iterator_bin)
    {
      local_bin->bin[iterator_bin] = boost::shared_ptr< RayCompressed[] >(new RayCompressed[local_bin->size]);
      local_bin->index[iterator_bin] = 0;
    }
    
    m_camera_threads.push_back(boost::shared_ptr< CameraThread >(new CameraThread(
      local_bin,
      m_batch,
      m_camera,
      m_sampler,
      m_image
      )));
    
    m_surface_threads.push_back(boost::shared_ptr< SurfaceThread >(new SurfaceThread(
      local_bin,
      m_batch,
      m_scene,
      m_image
      )));
  }
}

void Pathtracer::createCameraTasks()
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

  // std::cout << task_count_x * task_count_y << " render tasks created" << std::endl;
}

void Pathtracer::createSurfaceTasks()
{
  // size_t m_begin = 0;
  // size_t m_end = 2048;
  // while(m_array[m_begin] != m_array[m_end - 1])
  // {
  //   size_t s = m_begin;
  //   for(size_t i = m_begin; i < (m_end - 1); ++i)
  //   {
  //     if(m_array[i] != m_array[i + 1])
  //     {
  //       s = i + 1;
  //       break;
  //     }
  //   }

  //   m_surface_queue.push(task);
  // }
  // else if((m_end - m_begin) > 4096)
  // {
  //   size_t s = (m_end - m_begin) / 2;

  //   NewTask &lrange = *new( tbb::task::allocate_child() ) NewTask(m_begin, s, m_array);
  //   NewTask &rrange = *new( tbb::task::allocate_child() ) NewTask(s, m_end, m_array);

  //   tbb::task::set_ref_count(3);
    
  //   tbb::task::spawn(lrange);
  //   tbb::task::spawn(rrange);
  //   tbb::task::wait_for_all();
  // }
}

void Pathtracer::runCameraThreads()
{
  for(int i = 0; i < m_nthreads; ++i)
    m_camera_threads[i]->start(&m_camera_queue);

  for(int i = 0; i < m_nthreads; ++i)
    m_camera_threads[i]->join();
}

Pathtracer::Pathtracer(const std::string &_filename)
{
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

  rtcInit(NULL);

  construct(_filename);
  createThreads();
}

Pathtracer::~Pathtracer()
{
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
  // size_t pixel_count = m_image->width * m_image->height;
  // for(size_t i = 0; i < pixel_count; ++i)
  // {
  //   m_image->pixels[i].r += m_random.getSample();
  //   m_image->pixels[i].g += m_random.getSample();
  //   m_image->pixels[i].b += m_random.getSample();
  // }

  m_batch->construct(m_settings->batch_exponent);

  createCameraTasks();
  runCameraThreads();

  size_t batch_size = pow(2, m_settings->batch_exponent);
  RayCompressed* batch_current = new RayCompressed[batch_size];

  std::string path;
  while(m_batch->pop(&path))
  {
    std::ifstream infile;

    infile.open(path);
    infile.read((char *)batch_current, sizeof(RayCompressed) * batch_size);
    infile.close();

    BoundingBox3f limits;
    limits.min[0] = batch_current[0].org[0];
    limits.min[1] = batch_current[1].org[1];
    limits.min[2] = batch_current[2].org[2];
    limits.max[0] = batch_current[0].org[0];
    limits.max[1] = batch_current[1].org[1];
    limits.max[2] = batch_current[2].org[2];

    for(size_t iterator = 0; iterator < batch_size; ++iterator)
    {
      if(batch_current[iterator].org[0] < limits.min[0])
        limits.min[0] = batch_current[iterator].org[0];
      if(batch_current[iterator].org[1] < limits.min[1])
        limits.min[1] = batch_current[iterator].org[1];
      if(batch_current[iterator].org[2] < limits.min[2])
        limits.min[2] = batch_current[iterator].org[2];

      if(batch_current[iterator].org[0] > limits.max[0])
        limits.max[0] = batch_current[iterator].org[0];
      if(batch_current[iterator].org[1] > limits.max[1])
        limits.max[1] = batch_current[iterator].org[1];
      if(batch_current[iterator].org[2] > limits.max[2])
        limits.max[2] = batch_current[iterator].org[2];
    }

    RaySort &task = *new(tbb::task::allocate_root()) RaySort(0, batch_size, limits, batch_current);
    tbb::task::spawn_root_and_wait(task);

    tbb::parallel_for(
      Range64< size_t >(0, batch_size),
      RayIntersect(&(*m_scene), batch_current),
      tbb::simple_partitioner()
      );

    boost::filesystem::remove(path);
  }

  delete[] batch_current;

  m_batch->clear();

  m_image->iteration += 1;
  return m_image->iteration;
}

MSC_NAMESPACE_END