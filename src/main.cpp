#include <stdlib.h>
#include <iostream>
#include <string>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <OpenEXR/ImfRgbaFile.h>

#include <framebuffer/Framebuffer.h>
#include <core/Pathtracer.h>

namespace program_options = boost::program_options;
namespace filesystem = boost::filesystem;

void commands(const int ac, const char *av[], bool* file, filesystem::path* input)
{
  *file = false;

  program_options::options_description visible("options");
  visible.add_options()
    ("help,h", "produce help message")
    ("file,f", "output as file")
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

    if(vm.count("file"))
      *file = true;

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

int main(int argc, const char *argv[])
{
  bool type_file;
  filesystem::path input_file;

  commands(argc, argv, &type_file, &input_file);

  float* image_pointer;
  int width;
  int height;

  msc::Pathtracer* pathtracer = new msc::Pathtracer(input_file.string());
  pathtracer->image(&image_pointer, &width, &height);

  // pathtracer process here...
  size_t iteration = pathtracer->process();

  if(!type_file)
  {
    frm::Framebuffer* framebuffer = new frm::Framebuffer();
    framebuffer->init(width, height);

    size_t pixel_count = width * height;
    unsigned char* image = new unsigned char[pixel_count * 3];

    for(size_t i = 0; i < pixel_count; ++i)
    {
      image[3 * i + 0] = static_cast<unsigned char>(image_pointer[3 * i + 0] * 255 / iteration);
      image[3 * i + 1] = static_cast<unsigned char>(image_pointer[3 * i + 1] * 255 / iteration);
      image[3 * i + 2] = static_cast<unsigned char>(image_pointer[3 * i + 2] * 255 / iteration);
    }

    framebuffer->image(image, width, height);
    framebuffer->bind();

    boost::chrono::milliseconds iteration_second( 1000 / 60 );

    while(!framebuffer->close())
    {
      boost::chrono::high_resolution_clock::time_point timer_start = boost::chrono::high_resolution_clock::now();
      
      framebuffer->draw();

      std::string title = "Graphics Environment Iteration: " + std::to_string(iteration);
      framebuffer->title(title);

      // pathtracer iterate process here...
      iteration = pathtracer->process();

      for(size_t i = 0; i < pixel_count; ++i)
      {
        image[3 * i + 0] = static_cast<unsigned char>(image_pointer[3 * i + 0] * 255 / iteration);
        image[3 * i + 1] = static_cast<unsigned char>(image_pointer[3 * i + 1] * 255 / iteration);
        image[3 * i + 2] = static_cast<unsigned char>(image_pointer[3 * i + 2] * 255 / iteration);
      }

      framebuffer->image(image, width, height);

      boost::chrono::high_resolution_clock::time_point timer_end = boost::chrono::high_resolution_clock::now();
      boost::chrono::milliseconds iteration_time(boost::chrono::duration_cast<boost::chrono::milliseconds>
        (timer_end - timer_start).count());
      if(iteration_time < iteration_second)
      {
        boost::this_thread::sleep_for(iteration_second - iteration_time);
      }
    }

    delete[] image;
    delete framebuffer;
  }
  else
  {
    size_t pixel_count = width * height;
    Imf::Rgba* image = new Imf::Rgba[pixel_count];

    for(size_t i = 0; i < pixel_count; ++i)
    {
      image[i] = Imf::Rgba(image_pointer[i * 3 + 0], image_pointer[i * 3 + 1], image_pointer[i * 3 + 2], 1.f);
    }

    std::string output_file = filesystem::basename(input_file).append(".exr");
    Imf::RgbaOutputFile* file = new Imf::RgbaOutputFile(output_file.c_str(), width, height, Imf::WRITE_RGBA);
    file->setFrameBuffer(image, 1, width);
    file->writePixels(height);

    std::cout << "output file named '" << output_file << "' writen to working directory" << std::endl;

    delete[] image;
    delete file;
  }
  
  delete pathtracer;

  return EXIT_SUCCESS;
}