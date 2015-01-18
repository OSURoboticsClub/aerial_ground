#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#define GLSL(src) "#version 150 core\n" #src

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include <boost/asio.hpp>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/decoder.hpp"
#include "protocol/encoder.hpp"
#include "test.hpp"

#include "reader.hpp"
#include "writer.hpp"

float vertices[] = {
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.1f, 0.1f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int main(int argc, char **argv) {
  if(argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <tty>" << std::endl;
    return EXIT_FAILURE;
  }
  
  boost::asio::io_service io;
  boost::asio::serial_port port(io, argv[1]);

  port.set_option(boost::asio::serial_port_base::baud_rate(115200));
  port.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
  port.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
  port.set_option(boost::asio::serial_port::character_size(8));

  std::thread r(reader, std::ref(port));
  std::thread w(writer, std::ref(port));

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  // Create Vertex Array Object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a Vertex Buffer Object and copy the vertex data to it
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  const char* vertexSource = GLSL(
    in vec3 position;
    in vec3 color;

    out vec3 Color;

    void main() {
      Color = color;
      gl_Position = vec4(position, 1.0);
    }
  );
  
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Create and compile the fragment shader
  const char* fragmentSource = GLSL(
    in vec3 Color;
    out vec4 outColor;

    void main() {
      outColor = vec4(Color, 1.0);
    }
  );


  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Link the vertex and fragment shader into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

  while(!glfwWindowShouldClose(window)) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_LINES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }
  }

  // Delete allocated resources
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glfwTerminate();

  r.join();
  w.join();
}
