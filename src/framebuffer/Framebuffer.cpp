#include <iostream>

#include <framebuffer/Framebuffer.h>

FRM_NAMESPACE_BEGIN

Framebuffer::~Framebuffer()
{
  glDeleteTextures(1, &m_texture);
  glDeleteProgram(m_shader_program);
  glDeleteShader(m_vertex_shader);
  glDeleteShader(m_fragment_shader);
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

GLFWwindow* Framebuffer::init(const int _resolution_x, const int _resolution_y, void* _input_data)
{
  m_resolution_x = _resolution_x;
  m_resolution_y = _resolution_y;

  createContext();
  createSurface();

  if(_input_data != NULL)
  {
    glfwSetWindowUserPointer(m_window, _input_data);
  }
  else
  {
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetCursorPosCallback(m_window, cursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
  }

  return m_window;
}

void Framebuffer::bind()
{
  glfwMakeContextCurrent(m_window);

  glUseProgram(m_shader_program);
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Framebuffer::draw()
{
  glEnable(GL_FRAMEBUFFER_SRGB);
  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  glfwSwapBuffers(m_window);
  glfwPollEvents();

  if(m_update)
  {
    m_update = false;
    glUniform2f(m_trans_uniform, m_trans_x, m_trans_y);
    glUniform1f(m_scale_uniform, m_scale);
  }
}

bool Framebuffer::close()
{
  return glfwWindowShouldClose(m_window);
}


void Framebuffer::image(const unsigned char* _image, const int _resolution_x, const int _resolution_y)
{
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _resolution_x, _resolution_y, GL_RGB, GL_UNSIGNED_BYTE, _image);
}

void Framebuffer::title(const std::string &_title)
{
  glfwSetWindowTitle(m_window, _title.c_str());
}

void Framebuffer::createSurface()
{
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  float vertices[] = {
    -1.f,  1.f, 0.f, 1.f,
     1.f,  1.f, 1.f, 1.f,
     1.f, -1.f, 1.f, 0.f,

     1.f, -1.f, 1.f, 0.f,
    -1.f, -1.f, 0.f, 0.f,
    -1.f,  1.f, 0.f, 1.f
  };

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  const GLchar* vertex_source =
    "#version 330\n"
    "in vec2 position;"
    "in vec2 coordinate;"
    "uniform vec2 translation;"
    "uniform float scale;"
    "out vec2 Coordinate;"
    "void main()"
    "{"
    "   Coordinate = coordinate;"
    "   gl_Position = vec4((position + translation) * scale, 0.0, 1.0);"
    "}";

  const GLchar* fragment_source =
    "#version 330\n"
    "in vec2 Coordinate;"
    "uniform sampler2D tex;"
    "out vec4 outColor;"
    "void main()"
    "{"
    "   outColor = texture(tex, Coordinate);"
    "}";

  m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(m_vertex_shader, 1, &vertex_source, NULL);
  glCompileShader(m_vertex_shader);

  m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(m_fragment_shader, 1, &fragment_source, NULL);
  glCompileShader(m_fragment_shader);

  m_shader_program = glCreateProgram();
  glAttachShader(m_shader_program, m_vertex_shader);
  glAttachShader(m_shader_program, m_fragment_shader);
  glLinkProgram(m_shader_program);
  glUseProgram(m_shader_program);

  m_pos_attrib = glGetAttribLocation(m_shader_program, "position");
  glVertexAttribPointer(m_pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(m_pos_attrib);

  m_tex_attrib = glGetAttribLocation(m_shader_program, "coordinate");
  glVertexAttribPointer(m_tex_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(m_tex_attrib);

  m_trans_uniform = glGetUniformLocation(m_shader_program, "translation");
  glUniform2f(m_trans_uniform, m_trans_x, m_trans_y);

  m_scale_uniform = glGetUniformLocation(m_shader_program, "scale");
  glUniform1f(m_scale_uniform, m_scale);

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_resolution_x, m_resolution_y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
}

void Framebuffer::createContext()
{
  int glfw_error = glfwInit();
  if(glfw_error != GL_TRUE)
  {
    std::cout << "GLFW error: " << glfw_error << std::endl;
    exit(EXIT_FAILURE);
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  m_window = glfwCreateWindow(m_resolution_x, m_resolution_y, "Graphics Environment", NULL, NULL);

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(true);
  
  GLenum opengl_error = glGetError();
  if(opengl_error != GL_NO_ERROR)
  {
    std::cout << "OpenGL error: " << opengl_error << std::endl;
    exit(EXIT_FAILURE);
  }
  
  #ifdef USING_GLEW
  glewExperimental = GL_TRUE;
  GLenum glew_error = glewInit();
  if(glew_error != GLEW_OK)
  {
    std::cout << "Glew error: " << glew_error << std::endl;
    exit(EXIT_FAILURE);
  }
  #endif
}

void Framebuffer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  Framebuffer *framebuffer = static_cast<Framebuffer*>(glfwGetWindowUserPointer(window));

  if(action == GLFW_PRESS)
  {
    framebuffer->m_update = true;
    switch(key)
    {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
      case GLFW_KEY_F:
        framebuffer->m_trans_x = 0.f;
        framebuffer->m_trans_y = 0.f;
        framebuffer->m_scale = 1.f;
        break;
    }
  }
}

void Framebuffer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  Framebuffer *framebuffer = static_cast<Framebuffer*>(glfwGetWindowUserPointer(window));

  float scale = framebuffer->m_scale;
  scale += scale * yoffset * 0.05f;
  if(scale > 0.02f && scale < 20.f)
  {
    framebuffer->m_update = true;
    framebuffer->m_scale += yoffset * scale * 0.05f;
  }
}

void Framebuffer::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
  Framebuffer *framebuffer = static_cast<Framebuffer*>(glfwGetWindowUserPointer(window));

  if(framebuffer->m_pan)
  {
    framebuffer->m_update = true;
    float scale = 2.f / framebuffer->m_scale;
    framebuffer->m_trans_x = framebuffer->m_state_x + ((xpos - framebuffer->m_screen_x) / framebuffer->m_resolution_x) * scale;
    framebuffer->m_trans_y = framebuffer->m_state_y + ((framebuffer->m_screen_y - ypos) / framebuffer->m_resolution_y) * scale;
  }
}

void Framebuffer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  Framebuffer *framebuffer = static_cast<Framebuffer*>(glfwGetWindowUserPointer(window));

  if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    framebuffer->m_pan = true;

    framebuffer->m_state_x = framebuffer->m_trans_x;
    framebuffer->m_state_y = framebuffer->m_trans_y;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    framebuffer->m_screen_x = xpos;
    framebuffer->m_screen_y = ypos;
  }
  else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
  {
    framebuffer->m_pan = false;
  }
}

FRM_NAMESPACE_END