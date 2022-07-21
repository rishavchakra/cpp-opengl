#pragma once

#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

enum ShaderType { Vertex, Fragment };

class Shader {
public:
  unsigned int id;
  ShaderType shaderType;

  Shader(const char *shaderSource, ShaderType);

private:
  GLenum toGLEnum(ShaderType shader_type);
};

class ShaderProgram {
private:
  std::map<std::string, GLint> uniforms;

  GLint getUniformLocation(const std::string);

public:
  unsigned int id;

  ShaderProgram();
  ~ShaderProgram();

  void addShader(Shader);
  void addShader(unsigned int id);
  void compile();

  void use();

  // Uniform operations
  void addUniform(const std::string);
  void setBool(const std::string, bool);
  void setInt(const std::string, int);
  void setFloat(const std::string, float);
  void set2i(const std::string, int, int);
  void set2f(const std::string, float, float);
  void set3i(const std::string, int, int, int);
  void set3f(const std::string, float, float, float);
};

////////////////////////////////
//////// Method implementations
////////////////////////////////

Shader::Shader(const char *shaderSource, ShaderType shaderType) {
  // Reading the file contents to a string
  std::ifstream file_stream(shaderSource);
  if (!file_stream.is_open()) {
    std::cerr << "ERROR: Couldn't open shader file " << shaderSource
              << std::endl;
  }
  std::string shaderCode =
      std::string(std::istreambuf_iterator<char>(file_stream),
                  std::istreambuf_iterator<char>());
  const char *shaderCodeCstr = shaderCode.c_str();
  file_stream.close();

  // Creating shader, adding source, compiling shader
  this->id = glCreateShader(toGLEnum(shaderType));
  glShaderSource(this->id, 1, &shaderCodeCstr, NULL);
  glCompileShader(this->id);

  // Compile error checking
  int success;
  char infoLog[512];
  glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(this->id, 512, NULL, infoLog);
    std::cerr << "ERROR: Failed to compile shader " << shaderSource << "\n"
              << infoLog << std::endl;
  }
}

GLenum Shader::toGLEnum(ShaderType shader_type) {
  switch (shader_type) {
  case ShaderType::Vertex:
    return GL_VERTEX_SHADER;
  case ShaderType::Fragment:
    return GL_FRAGMENT_SHADER;
  default:
    return (GLenum)NULL;
  }
}

ShaderProgram::ShaderProgram() { this->id = glCreateProgram(); }

ShaderProgram::~ShaderProgram() { glDeleteProgram(this->id); }

void ShaderProgram::addShader(Shader shader) {
  glAttachShader(this->id, shader.id);
}

void ShaderProgram::addShader(unsigned int id) { glAttachShader(this->id, id); }

void ShaderProgram::compile() {
  glLinkProgram(this->id);

  int success;
  glGetProgramiv(this->id, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(this->id, 512, NULL, infoLog);
    std::cerr << "ERROR: Failed to link shader program\n"
              << infoLog << std::endl;
  }
}

void ShaderProgram::use() { glUseProgram(this->id); }

////////////////////////////////
//////// Uniform methods
////////////////////////////////

void ShaderProgram::addUniform(const std::string uniform_name) {
  GLint uniformLocation = glGetUniformLocation(this->id, uniform_name.c_str());
  if (uniformLocation == -1) {
    std::cerr << "ERROR: Uniform not found: " << uniform_name << std::endl;
    return;
  }
  this->uniforms[uniform_name] = uniformLocation;
}

//////// Uniform setters

void ShaderProgram::setInt(const std::string uniform_name, int value) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform1i(uniformLocation, value);
}

void ShaderProgram::setBool(const std::string uniform_name, bool value) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform1i(uniformLocation, (int)value);
}

void ShaderProgram::setFloat(const std::string uniform_name, float value) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform1f(uniformLocation, value);
}

void ShaderProgram::set2i(const std::string uniform_name, int val0, int val1) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform2i(uniformLocation, val0, val1);
}

void ShaderProgram::set2f(const std::string uniform_name, float val0,
                          float val1) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform2f(uniformLocation, val0, val1);
}

void ShaderProgram::set3i(const std::string uniform_name, int val0, int val1,
                          int val2) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform3i(uniformLocation, val0, val1, val2);
}

void ShaderProgram::set3f(const std::string uniform_name, float val0,
                          float val1, float val2) {
  GLint uniformLocation = this->getUniformLocation(uniform_name);
  glUniform3f(uniformLocation, val0, val1, val2);
}

GLint ShaderProgram::getUniformLocation(const std::string uniform_name) {
  GLint uniformLocation;
  try {
    uniformLocation = this->uniforms.at(uniform_name);
  } catch (int e) {
    std::cerr << "ERROR: Could not find uniform " << uniform_name << std::endl;
  }
  return uniformLocation;
}