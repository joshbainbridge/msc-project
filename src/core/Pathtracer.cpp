#include <boost/thread.hpp>
#include <boost/weak_ptr.hpp>

#include <core/Pathtracer.h>
#include <core/LocalBin.h>
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

  if(m_nthreads > 1)
    std::cout << m_nthreads << " supported threads found" << std::endl;

  if(!(m_nthreads > 1))
    std::cout << "Single thread found, system will function as a serial operation" << std::endl;

  for (size_t i = 0; i < m_nthreads; ++i)
  {
    boost::shared_ptr< LocalBin > local_bin(new LocalBin());
    m_camera_threads.push_back(boost::shared_ptr< CameraThread >(new CameraThread(
      m_camera_queue,
      local_bin,
      m_bin,
      m_camera,
      m_sampler
      )));
    m_surface_threads.push_back(boost::shared_ptr< SurfaceThread >(new SurfaceThread(
      m_surface_queue,
      local_bin,
      m_bin,
      m_scene,
      m_image
      )));
  }
}

void Pathtracer::createCameraTasks()
{
  size_t bucket_size = m_settings->bucket_size;
  size_t width = m_image->width;
  size_t height = m_image->height;
  size_t task_count_x = ceil(width / static_cast<float>(bucket_size));
  size_t task_count_y = ceil(height / static_cast<float>(bucket_size));
  size_t task_count = task_count_x * task_count_y;

  std::cout << task_count << " render tasks created" << std::endl;

  for (size_t i = 0; i < task_count_x; ++i)
  {
    for (size_t j = 0; j < task_count_y; ++j)
    {
      //Check for any pixels left after resolution division with bucket size
      size_t task_width = bucket_size;
      if ((i * bucket_size + bucket_size) > task_width)
        task_width = task_width - (i * bucket_size);

      //Check for any pixels left after resolution division with bucket size
      size_t task_height = bucket_size;
      if ((j * bucket_size + bucket_size) > task_height)
        task_height = task_height - (j * bucket_size);

      CameraTask task;
      task.begin_x = i * bucket_size;
      task.begin_y = j * bucket_size;
      task.end_x = task.begin_x + task_width;
      task.end_y = task.begin_y + task_height;

      m_camera_queue.push(task);
    }
  }
}

void Pathtracer::createSurfaceTasks()
{

}

void Pathtracer::runCameraThreads()
{
  for (int i = 0; i < m_nthreads; ++i)
    m_camera_threads[i]->start();

  for (int i = 0; i < m_nthreads; ++i)
    m_camera_threads[i]->join();
}

Pathtracer::Pathtracer(const std::string &_filename)
{
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

  construct(_filename);

  createThreads();
  createCameraTasks(); // Move this to process method
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

  m_settings->iteration = 0;
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