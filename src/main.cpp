#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <string>

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

// Read file contents into a stream
const char* readFileToString(std::string filename) {
  std::ifstream ifs(filename);
  std::string content;
  content.assign( (std::istreambuf_iterator<char>(ifs) ),
    (std::istreambuf_iterator<char>()));
  ifs.close();
  return strdup(content.c_str());
}

int main()
{
  // Shader reading
  const char* vertexShaderSource = readFileToString("shaders/vertexSimple.vert.glsl");
  const char* fragmentShaderSource = readFileToString("shaders/fragRed.frag.glsl");
  const char* fragmentShaderSource2 = readFileToString("shaders/fragPink.frag.glsl");
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

  // Second fragment shader creation
  unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);

  // Shader compilation error checking
  int successVert;
  int successFrag;
  int successFrag2;
  char infoLogVert[512];
  char infoLogFrag[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFrag);
  glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &successFrag2);
  if (!successVert || !successFrag || !successFrag2)
  { // Shader compilation unsuccessful
    std::cout << "Vertex Shader status: " << successVert << "\n"
              << "Fragment Shader status: " << successFrag << "\n"
              << "Fragment Shader 2 status: " << successFrag2 << std::endl;
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

  unsigned int shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader); // Reusing the same vertex shader; no change
  glAttachShader(shaderProgram2, fragmentShader2); // New fragment shader
  glLinkProgram(shaderProgram2);

  // Shader program linking error checking
  int successLink;
  int successLink2;
  char infoLogLink[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successLink);
  glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &successLink2);
  if (!successLink || !successLink2)
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
  glDeleteShader(fragmentShader2);

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

  float verticesTriangle1[] = {
    -0.7f, -0.7f, 0.0f,
    -0.3f, -0.7f, 0.0f,
    -0.7f, -0.3f, 0.0f,
  };
  float verticesTriangle2[] = {
    0.7f, 0.7f, 0.0f,
    0.3f, 0.7f, 0.0f,
    0.7f, 0.3f, 0.0f,
  };

  /*
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
  */

  // Drawing two triangles with separate VAOs and VBOs
  unsigned int VBO1, VAO1, VBO2, VAO2;
  glGenVertexArrays(1, &VAO1);
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &VBO2);
  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle1), verticesTriangle1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle2), verticesTriangle2, GL_STATIC_DRAW);
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

    /*
    glBindVertexArray(VAO);           // Binds the vertex array
    // glDrawArrays(GL_TRIANGLES, 0, 3); // Draws from the currently bound buffer
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    */

    // Drawing two triangles with two different VAO/VBOs
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}
