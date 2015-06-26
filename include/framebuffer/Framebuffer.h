#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <string>

#include <framebuffer/PlatformSpecification.h>

#define FRB_NAMESPACE_BEGIN namespace frb {
#define FRB_NAMESPACE_END }

FRB_NAMESPACE_BEGIN

class Framebuffer
{
public:
  Framebuffer()
    : m_update(true)
    , m_pan(false)
    , m_res_x(700.f)
    , m_res_y(500.f)
    , m_screen_x(0.f)
    , m_screen_y(0.f)
    , m_state_x(0.f)
    , m_state_y(0.f)
    , m_trans_x(0.f)
    , m_trans_y(0.f)
    , m_scale(1.f)
  {;}

  ~Framebuffer();
  GLFWwindow* init(const int _resx, const int _resy, void* input_data = NULL);
  void bind();
  void draw();
  bool close();
  void image(const float *_image, const int _resx, const int _resy);
  void title(const std::string &_title);

private:
  GLFWwindow* m_window;
  GLuint m_texture;
  GLuint m_shader_program;
  GLuint m_vertex_shader;
  GLuint m_fragment_shader;
  GLuint m_vbo;
  GLuint m_vao;
  GLint m_pos_attrib;
  GLint m_tex_attrib;
  GLint m_trans_uniform;
  GLint m_scale_uniform;

  void createContext();
  void createSurface();
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
  static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
  bool m_update;
  bool m_pan;
  float m_res_x, m_res_y;
  float m_screen_x, m_screen_y;
  float m_state_x, m_state_y;
  float m_trans_x, m_trans_y;
  float m_scale;
};

FRB_NAMESPACE_END

#endif