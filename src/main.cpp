#include <stdlib.h>
#include <iostream>
#include <string>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <OpenEXR/ImfRgbaFile.h>

#include <framebuffer/Framebuffer.h>
#include <core/Pathtracer.h>

namespace program_options = boost::program_options;
namespace filesystem = boost::filesystem;

void commands(const int ac, const char *av[], filesystem::path* input, filesystem::path* output)
{
  program_options::options_description visible("options");
  visible.add_options()
    ("help,h", "produce help message")
    ("output,o", program_options::value< filesystem::path >(), "output as file")
  ;

  program_options::options_description hidden("hidden options");
  hidden.add_options()
    ("input", program_options::value< filesystem::path >()->required(), "input scene file")
  ;

  program_options::options_description description("all options");
  description.add(visible).add(hidden);

  program_options::positional_options_description positional;
  positional.add("input", -1);
 
  program_options::variables_map vm;

  try
  {
    program_options::store(
      program_options::command_line_parser(ac, av).options(description).positional(positional).run(), vm);

    if(vm.count("help"))
    {
      std::cout << "usage: msc-project [options] scene_file.yaml" << std::endl;
      std::cout << visible << std::endl;
      std::exit(EXIT_SUCCESS);
    }

    program_options::notify(vm);

    if(vm.count("output"))
      *output = vm["output"].as< filesystem::path >();

    if(vm.count("input"))
      *input = vm["input"].as< filesystem::path >();
  }
  catch(program_options::required_option& error)
  {
    std::cerr << "error: " << "a scene file is required but missing" << std::endl << std::endl;
    std::cerr << "usage: msc-project [options] scene_file.yaml" << std::endl;
    std::cerr << visible << std::endl;
    std::exit(EXIT_FAILURE);
  }
  catch(program_options::error& error)
  {
    std::cerr << "error: " << error.what() << std::endl << std::endl;
    std::cerr << "usage: msc-project [options] scene_file.yaml" << std::endl;
    std::cerr << visible << std::endl;
    std::exit(EXIT_FAILURE);
  }

  if(!filesystem::exists( *input ))
  {
    std::cerr << "error: " << "the scene file does not exist" << std::endl << std::endl;
    std::cerr << "usage: msc-project [options] scene_file.yaml" << std::endl;
    std::cerr << visible << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  msc::Pathtracer* pathtracer = static_cast<msc::Pathtracer*>(glfwGetWindowUserPointer(window));

  if(action == GLFW_PRESS)
  {
    switch(key)
    {
      case GLFW_KEY_ESCAPE:
        pathtracer->terminate();
        break;
    }
  }
}

void closeCallback(GLFWwindow* window)
{
  msc::Pathtracer* pathtracer = static_cast<msc::Pathtracer*>(glfwGetWindowUserPointer(window));
  pathtracer->terminate();
}

void compute(msc::Pathtracer* pathtracer, size_t* iteration)
{
  boost::chrono::milliseconds iteration_second( 1000 / 30 );

  while(pathtracer->active())
  {
    boost::chrono::high_resolution_clock::time_point timer_start = boost::chrono::high_resolution_clock::now();

    *iteration = pathtracer->process();

    if(*iteration == 0)
      break;

    std::cout << "\033[1;31mIteration " << *iteration << " is complete.\033[0m" << std::endl;

    boost::chrono::high_resolution_clock::time_point timer_end = boost::chrono::high_resolution_clock::now();
    boost::chrono::milliseconds iteration_time(boost::chrono::duration_cast<boost::chrono::milliseconds>
      (timer_end - timer_start).count());
    if(iteration_time < iteration_second)
      boost::this_thread::sleep_for(iteration_second - iteration_time);
  }
}

int main(int argc, const char *argv[])
{
  filesystem::path input_file;
  filesystem::path output_file;

  commands(argc, argv, &input_file, &output_file);

  float* image_pointer;
  int width, height;

  msc::Pathtracer* pathtracer = new msc::Pathtracer(input_file.string());
  pathtracer->image(&image_pointer, &width, &height);

  size_t pixel_count = width * height;

  if(output_file.empty())
  {
    frm::Framebuffer* framebuffer = new frm::Framebuffer();
    GLFWwindow* window = framebuffer->init(width, height, pathtracer);

    size_t iteration = 0;
    size_t check_it = 0;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowCloseCallback(window, closeCallback);

    framebuffer->bind();
    framebuffer->poll();

    unsigned char* image = new unsigned char[pixel_count * 3];

    boost::thread compute_thread = boost::thread(&compute, pathtracer, &iteration);

    boost::chrono::milliseconds iteration_second( 1000 / 30 );

    while(pathtracer->active())
    {
      boost::chrono::high_resolution_clock::time_point timer_start = boost::chrono::high_resolution_clock::now();

      if(check_it != iteration)
      {
        check_it = iteration;
        for(size_t i = 0; i < pixel_count; ++i)
        {
          image[3 * i + 0] = static_cast<unsigned char>(std::min(1.f, image_pointer[3 * i + 0] / iteration) * 255.f);
          image[3 * i + 1] = static_cast<unsigned char>(std::min(1.f, image_pointer[3 * i + 1] / iteration) * 255.f);
          image[3 * i + 2] = static_cast<unsigned char>(std::min(1.f, image_pointer[3 * i + 2] / iteration) * 255.f);
        }

        std::string title = "Pathtracer Iteration: " + boost::lexical_cast<std::string>(iteration);

        framebuffer->title(title);
        framebuffer->image(image, width, height);
      }

      framebuffer->draw();
      framebuffer->poll();

      boost::chrono::high_resolution_clock::time_point timer_end = boost::chrono::high_resolution_clock::now();
      boost::chrono::milliseconds iteration_time(boost::chrono::duration_cast<boost::chrono::milliseconds>
        (timer_end - timer_start).count());
      if(iteration_time < iteration_second)
        boost::this_thread::sleep_for(iteration_second - iteration_time);
    }

    compute_thread.join();
  
    delete[] image;
    delete framebuffer;
  }
  else
  {
    size_t iteration = pathtracer->process();

    Imf::Rgba* image = new Imf::Rgba[pixel_count];

    for(size_t i = 0; i < pixel_count; ++i)
      image[i] = Imf::Rgba(image_pointer[i * 3 + 0], image_pointer[i * 3 + 1], image_pointer[i * 3 + 2], 1.f);

    Imf::RgbaOutputFile* file = new Imf::RgbaOutputFile(output_file.c_str(), width, height, Imf::WRITE_RGBA);
    file->setFrameBuffer(image, 1, width);
    file->writePixels(height);

    std::cout << "\033[1;31mOutput file named " << output_file << " written to working directory.\033[0m" << std::endl;

    delete[] image;
    delete file;
  }
  
  delete pathtracer;

  return EXIT_SUCCESS;
}