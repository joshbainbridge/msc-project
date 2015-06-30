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

void commands(const int ac, const char *av[], std::string* output, filesystem::path* input)
{
  program_options::options_description visible("options");
  visible.add_options()
    ("help,h", "Produce help message")
    ("output-type,t", program_options::value< std::string >(), "Either 'framebuffer' or 'file' output")
  ;

  program_options::options_description hidden("hidden options");
  hidden.add_options()
    ("input-file,i", program_options::value< filesystem::path >(), "Input scene file")
  ;

  program_options::options_description description("all options");
  description.add(visible).add(hidden);

  program_options::positional_options_description positional;
  positional.add("input-file", -1);
 
  program_options::variables_map vm;
  program_options::store(
    program_options::command_line_parser(ac, av).options(description).positional(positional).run(), vm);
  program_options::notify(vm);

  if(vm.count("help"))
  {
    std::cout << "usage: msc-project [options] scene_file.yaml" << std::endl;
    std::cout << visible << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  if(vm.count("output-type"))
  {
    *output = vm["output-type"].as< std::string >();
    if(*output != "framebuffer" && *output != "file")
    {
      std::cerr << "usage: msc-project [options] scene_file.yaml" << std::endl;
      std::cerr << visible << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  if(vm.count("input-file"))
  {
    *input = vm["input-file"].as< filesystem::path >();
  }
  else
  {
    std::cerr << "usage: msc-project [options] scene_file.yaml" << std::endl;
    std::cerr << visible << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

int main(int argc, const char *argv[])
{
  std::string output_type("framebuffer");
  filesystem::path input_file("");

  commands(argc, argv, &output_type, &input_file);

  float* image_pointer;
  int width;
  int height;

  msc::Pathtracer* pathtracer = new msc::Pathtracer(input_file.string());
  pathtracer->image(&image_pointer, &width, &height);

  if(output_type == "framebuffer")
  {
    frm::Framebuffer* framebuffer = new frm::Framebuffer();
    framebuffer->init(width, height);

    framebuffer->image(image_pointer, width, height);
    framebuffer->bind();

    unsigned int iteration = 0;
    boost::chrono::milliseconds iteration_second( 1000 / 60 );

    while(!framebuffer->close())
    {
      boost::chrono::high_resolution_clock::time_point timer_start = boost::chrono::high_resolution_clock::now();
      
      framebuffer->draw();

      std::string title = "Graphics Environment Iteration: " + std::to_string(++iteration);
      framebuffer->title(title);

      // pathtracer process here...
      framebuffer->image(image_pointer, width, height);

      boost::chrono::high_resolution_clock::time_point timer_end = boost::chrono::high_resolution_clock::now();
      boost::chrono::milliseconds iteration_time(boost::chrono::duration_cast<boost::chrono::milliseconds>
        (timer_end - timer_start).count());
      if(iteration_time < iteration_second)
      {
        boost::this_thread::sleep_for(iteration_second - iteration_time);
      }
    }
  }
  else if(output_type == "file")
  {
    int pixel_count = width * height;
    Imf::Rgba* image = new Imf::Rgba[pixel_count];

    for(int i = 0; i < pixel_count; ++i)
    {
      image[i] = Imf::Rgba(image_pointer[i * 3 + 0], image_pointer[i * 3 + 1], image_pointer[i * 3 + 2], 1.f);
    }

    std::string output_file = filesystem::basename(input_file).append(".exr");
    Imf::RgbaOutputFile* file = new Imf::RgbaOutputFile(output_file.c_str(), width, height, Imf::WRITE_RGBA);
    file->setFrameBuffer(image, 1, width);
    file->writePixels(height);

    std::cout << "Output file named '" << output_file << "' writen to working directory" << std::endl;

    delete[] image;
    delete file;
  }

  return EXIT_SUCCESS;
}