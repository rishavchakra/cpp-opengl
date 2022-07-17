#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

// Callback for window resizing
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// Key input
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.xyz, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(0.8f, 0.4f, 0.3f, 1.0f);\n"
    "}\n\0";
// const char *fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//     "}\n\0";

int main()
{
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
  if (window == NULL)
  {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }
  std::cout << "Created window" << std::endl;
  glfwMakeContextCurrent(window);
  // Callback for frame resizing
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD initializer
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Constructs a viewport with initial size 800x600
  // glViewport(0, 0, 800, 600);

  // Vertex shader creation
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Fragment shader creation
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Shader compilation error checking
  int successVert;
  int successFrag;
  char infoLogVert[512];
  char infoLogFrag[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
  if (!successVert || !successFrag)
  { // Shader compilation unsuccessful
    std::cout << "Vertex Shader status: " << successVert << "\n"
              << "Fragment Shader status: " << successFrag << std::endl;
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVert);
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFrag);
    std::cout << "ERROR: Shader compilation failed\n"
              << "Vertex Info Log: " << infoLogVert << "\n"
              << "Fragment Info Log: " << infoLogFrag << std::endl;

    return 1;
  }

  // Creating the shader program that links the vertex and fragment shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Shader program linking error checking
  int successLink;
  char infoLogLink[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successLink);
  if (!successLink)
  {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLogLink);
    std::cout << "ERROR: Shader linking failed\n"
              << "Linking Info Log: \n" 
              << infoLogLink << std::endl;

    return 1;
  }

  // Telling GL to use the shader program as the shader pipeline
  glUseProgram(shaderProgram);
  // Deleting the vertex and fragment shaders
  // because they're already linked into the program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // GL square drawing
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f
      };
  unsigned int indices[] = {
    0, 2, 1,
    0, 3, 2
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // Enabling vertex attributes (none by default)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Run loop
  while (!glfwWindowShouldClose(window))
  {
    // Process input events
    processInput(window);

    // Rendering instructions
    glClearColor(0.3f, 0.6f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);      // Sets the shader program to be used
    glBindVertexArray(VAO);           // Binds the vertex array
    // glDrawArrays(GL_TRIANGLES, 0, 3); // Draws from the currently bound buffer
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap buffers and polling
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Final process termination
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}