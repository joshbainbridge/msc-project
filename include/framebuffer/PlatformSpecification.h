#ifndef _PLATFORM_SPECIFICATION_H_
#define _PLATFORM_SPECIFICATION_H_

#ifdef __APPLE__
  #define GLFW_INCLUDE_GLCOREARB
  #include <GLFW/glfw3.h>
#elif __linux__
  #define USING_GLEW
  #define GLEW_STATIC
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#elif _WIN32
  #define USING_GLEW
  #define GLEW_STATIC
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif

#endif