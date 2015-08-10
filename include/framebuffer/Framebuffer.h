#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <string>

#include <framebuffer/PlatformSpecification.h>

#define FRM_NAMESPACE_BEGIN namespace frm {
#define FRM_NAMESPACE_END }

FRM_NAMESPACE_BEGIN

/**
 * @brief      Generic framebuffer for visualizing images
 * 
 * Framebuffer that uses OpenGL and GLFW to visualize images. It will take a void pointer to data
 * that can then be acceded within key callbacks externally. If a void pointer is not passed then
 * default input is used. 
 */
class Framebuffer
{
public:
  /**
   * @brief      Initialiser list for class
   */
  Framebuffer()
    : m_update(true)
    , m_pan(false)
    , m_resolution_x(700.f)
    , m_resolution_y(500.f)
    , m_screen_x(0.f)
    , m_screen_y(0.f)
    , m_state_x(0.f)
    , m_state_y(0.f)
    , m_trans_x(0.f)
    , m_trans_y(0.f)
    , m_scale(1.f)
  {;}

  /**
   * @brief      Class destructor for deleting buffers and destroying window
   */
  ~Framebuffer();

  /**
   * @brief      Initialize framebuffer with parameters and optional control class
   *
   * @param[in]  _resx        buffers horizontal resolution
   * @param[in]  _resy        buffers vertical resolution
   * @param      _input_data  optional void pointer to class for mutation when using external callback
   *
   * @return     window pointer to be used to set external key callbacks
   */
  GLFWwindow* init(const int _resx, const int _resy, void* _input_data = NULL);

  /**
   * @brief      Bind data before drawing in case of use with other contexts
   */
  void bind();

  /**
   * @brief      Draw image to framebuffer
   */
  void draw();

  /**
   * @brief      Test if window should close
   *
   * @return     boolean value
   */
  bool close();

  /**
   * @brief      Poll for changed events
   */
  void poll();

  /**
   * @brief      Set location of image data to be passed onto gpu
   *
   * @param[in]  _image     pointer to image data
   * @param[in]  _resx      horizontal resolution
   * @param[in]  _resy      vertical resolution
   */
  void image(const unsigned char* _image, const int _resx, const int _resy);

  /**
   * @brief      Set framebuffer window title to string
   *
   * @param[in]  _title  title
   */
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
  float m_resolution_x, m_resolution_y;
  float m_screen_x, m_screen_y;
  float m_state_x, m_state_y;
  float m_trans_x, m_trans_y;
  float m_scale;
};

FRM_NAMESPACE_END

#endif