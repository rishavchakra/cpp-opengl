#include "glad/glad.h"
// GLAD needs to be included before GLFW
#include "GLFW/glfw3.h"
#include <iostream>
#include <math.h>
#include <string>
#include <unistd.h>

#include "shader.hpp"

// Callback for window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Key input
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  // GLFW process initialization and flags
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // GLFW window initializer
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  std::cout << "Created window" << std::endl;
  glfwMakeContextCurrent(window);
  // Callback for frame resizing
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD initializer
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Constructs a viewport with initial size 800x600
  // glViewport(0, 0, 800, 600);

  Shader vertexShader =
      Shader("../../shaders/vertexSimple.vert.glsl", ShaderType::Vertex);
  Shader fragShaderRed =
      Shader("../../shaders/fragRed.frag.glsl", ShaderType::Fragment);
  Shader fragShaderPink =
      Shader("../../shaders/fragPink.frag.glsl", ShaderType::Fragment);

  // Creating the shader program that links the vertex and fragment shaders
  ShaderProgram shaderProgram = ShaderProgram();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragShaderRed);
  shaderProgram.compile();
  // does not need to Use the shader program to find location,
  // but does need to Use the shader program to update the value
  shaderProgram.addUniform("uni_color");
  shaderProgram.use();

  int count;
  glGetProgramiv(shaderProgram.id, GL_ACTIVE_UNIFORMS, &count);
  std::cout << "Active uniforms: " << count << std::endl;

  ShaderProgram shaderProgram2 = ShaderProgram();
  shaderProgram2.addShader(vertexShader);
  shaderProgram2.addShader(fragShaderPink);
  shaderProgram2.compile();

  glGetProgramiv(shaderProgram2.id, GL_ACTIVE_UNIFORMS, &count);
  std::cout << "Active uniforms: " << count << std::endl;

  // GL square drawing
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom left
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom right
      0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // Top right
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f  // Top left
  };
  unsigned int indices[] = {0, 2, 1, 0, 3, 2};

  // Two triangles buffers
  float verticesTriangle1[] = {
      -0.7f, -0.7f, 0.0f, -0.3f, -0.7f, 0.0f, -0.7f, -0.3f, 0.0f,
  };
  float verticesTriangle2[] = {
      0.7f, 0.7f, 0.0f, 0.3f, 0.7f, 0.0f, 0.7f, 0.3f, 0.0f,
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  // Bind Vertex Array Object
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // Copy the vertices array into another GPU buffer that OpenGL can use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // Enabling vertex attributes
  // Layout 0 = posiiton
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Layout 1 = color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /*
    // Drawing two triangles with separate VAOs and VBOs
    unsigned int VBO1, VAO1, VBO2, VAO2;
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle1), verticesTriangle1,
    GL_STATIC_DRAW); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *
    sizeof(float), (void *)0); glEnableVertexAttribArray(0);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle2), verticesTriangle2,
    GL_STATIC_DRAW); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *
    sizeof(float), (void *)0); glEnableVertexAttribArray(0);
  */

  // Run loop
  while (!glfwWindowShouldClose(window)) {
    // Process input events
    processInput(window);

    // Rendering instructions
    glClearColor(0.3f, 0.6f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use(); // Sets the shader program to be used

    glBindVertexArray(VAO); // Binds the vertex array
    // glDrawArrays(GL_TRIANGLES, 0, 3); // Draws from the currently bound
    // buffer
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /*
       // Fragment shader uniforms
       float time = glfwGetTime();
       float redVal = sin(time)/2.0f + 0.5f; // Modulating between 0 and 1
       // This needs to be done after the appropriate shader program is used
       glUniform4f(uniformFragColorLocation, redVal, 0.3f, 0.4f, 1.0f); // Pass
       the value from the CPU to GPU at the uniform location

        // Drawing two triangles with two different VAO/VBOs
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    */

    // Swap buffers and polling
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Final process termination
  /*
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  */
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
