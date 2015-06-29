#include <iostream>

#include <core/RenderThread.h>

MSC_NAMESPACE_BEGIN

RenderThread::RenderThread(boost::shared_ptr<Scene> _scene_ptr, boost::shared_ptr<Settings> _settings_ptr, boost::shared_ptr<Image> _image_ptr, boost::shared_ptr<TaskStack> _task_stack_ptr, boost::shared_ptr<IntegratorInterface> _integrator)
{
  m_scene_ptr = _scene_ptr;
  m_settings_ptr = _settings_ptr;
  m_image_ptr = _image_ptr;
  m_task_stack_ptr = _task_stack_ptr;
  m_integrator = _integrator;

  m_integrator->limitDepth(m_settings_ptr->depth);
  m_integrator->absorption(m_settings_ptr->absorption);

  //Construct uniform sampler to avoid scene mutation when rendering across multiple threads
  boost::shared_ptr<RandomGenerator> random_generator(new RandomGenerator());
  m_random_generator = random_generator;
}

//Start self as an independent thread
void RenderThread::start()
{
  m_thread = boost::thread(&RenderThread::process, this);
}

//Join thread
void RenderThread::join()
{
  m_thread.join();
}

//Main render function
void RenderThread::process()
{
  //if there are tasks available, take a new task from stack
  while (m_task_stack_ptr->takeTask(&m_task))
  {
    //Bucket to hold sample data
    std::vector< std::vector< std::vector< Colour3f > > > bucket;
    
    //Resize bucket as size is known
    bucket.resize(m_task.width);
    for (int i = 0; i < m_task.width; ++i)
    {
      bucket[i].resize(m_task.height);
      for (int j = 0; j < m_task.height; ++j)
      {
        bucket[i][j].resize(m_task.samples);
      }
    }

    //Loop through task pixels and samples
    for (int x_index = 0; x_index < m_task.width; ++x_index)
    {
      for (int y_index = 0; y_index < m_task.height; ++y_index)
      {
        for (int sample_index = 0; sample_index < m_task.samples; ++sample_index)
        {
          //Find image sample position
          float x_rand = m_random_generator->getValue() / static_cast<float>(m_settings_ptr->resolution_x);
          float x_pos = (static_cast<float>(x_index + m_task.pixel_x + 1) / static_cast<float>(m_settings_ptr->resolution_x)) * 2.f - 1.f - x_rand;
          float y_rand = m_random_generator->getValue() / static_cast<float>(m_settings_ptr->resolution_y);
          float y_pos = (static_cast<float>(y_index + m_task.pixel_y + 1) / static_cast<float>(m_settings_ptr->resolution_y)) * 2.f - 1.f - y_rand;
          
          //Pass position into camera to create innitial rays
          Vector3f ray_origin;
          Vector3f ray_direction;
          m_scene_ptr->getCamera()->makeSample(x_pos, y_pos, &ray_origin, &ray_direction, *m_random_generator);
          
          //Ray construction
          Ray3f camera_sample;
          camera_sample.origin = ray_origin;
          camera_sample.dir = ray_direction;
          
          //Use intergrator to find sample value
          Colour3f returned_value = m_integrator->run(m_scene_ptr, m_random_generator, camera_sample, Colour3f(1.f, 1.f, 1.f));
          bucket[x_index][y_index][sample_index] = returned_value;
        }
      }
    }
    
    //Add samples to image
    m_image_ptr->addBucket(m_task, bucket);
  }
}

MSC_NAMESPACE_END