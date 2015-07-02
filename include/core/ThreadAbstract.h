#ifndef _THREADABSTRACT_H_
#define _THREADABSTRACT_H_

#include <boost/thread.hpp>
#include <boost/weak_ptr.hpp>

#include <core/Common.h>
#include <core/Scene.h>
#include <core/Settings.h>
#include <core/Image.h>
#include <core/RandomGenerator.h>

MSC_NAMESPACE_BEGIN

class ThreadAbstract
{ 
public:
  ThreadAbstract(
      boost::weak_ptr<Scene> _scene_ptr,
      boost::weak_ptr<Settings> _settings_ptr,
      boost::weak_ptr<Image> _image_ptr
      // boost::weak_ptr<TaskStack> _task_stack_ptr,
      // boost::weak_ptr<IntegratorInterface> _integrator
      );

  void start();
  void join();
  void process();

private:
  boost::thread m_thread;
  boost::weak_ptr<Scene> m_scene_ptr;
  boost::weak_ptr<Settings> m_settings_ptr;
  boost::weak_ptr<Image> m_image_ptr;
  boost::weak_ptr<RandomGenerator> m_random_generator;
  // boost::shared_ptr<TaskStack> m_task_stack_ptr;
  // boost::shared_ptr<IntegratorInterface> m_integrator;
  // Task m_task;
};

MSC_NAMESPACE_END

#endif