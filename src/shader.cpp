#include "glad/glad.h"

#include "shader.h"
#include "common.h"
#include "fstream"



uint compile_shader(string const& path, uint type) {
  std::ifstream file(path);
  string code((std::istreambuf_iterator<char>(file)),
              std::istreambuf_iterator<char>());
  const char* ptr = code.data();
  uint id = glCreateShader(type);
  glShaderSource(id, 1, &ptr, NULL);
  glCompileShader(id);
  int len = 0, res = 1;
  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
  if (!res) {
    vector<char> err(len);
    glGetShaderInfoLog(id, len, 0, err.data());
    std::cout << "Failed to compile shader :\n";
    std::cout << err.data() << "\n";
  }
  return id;
}
void Shader::load(string vs, string ps) {
  id = glCreateProgram();

  glAttachShader(id, compile_shader(vs, GL_VERTEX_SHADER));
  glAttachShader(id, compile_shader(ps, GL_FRAGMENT_SHADER));
  glLinkProgram(id);

  int len = 0, res = 1;
  glGetProgramiv(id, GL_COMPILE_STATUS, &res);
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
  if (!res) {
    vector<char> err(len);
    glGetShaderInfoLog(id, len, 0, err.data());
    std::cout << "Failed to generate shader :\n";
    std::cout << err.data() << "\n";
    return;
  }
  glUseProgram(id);
}

void Shader::bind() {
  glUseProgram(id);
}

void Shader::set_uniform(const char* name, bool data) {
  glUniform1i(glGetUniformLocation(id, name), data);
}

void Shader::set_uniform(const char* name, glm::mat4 const& data) {
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, 0, &data[0][0]);
}

void Shader::set_uniform_array(const char* name,
                               const glm::mat4* data,
                               uint size) {
  glUniformMatrix4fv(glGetUniformLocation(id, name), size, 0, (float*)data);
}

void Shader::set_uniform(const char* name, glm::vec3 const& data) {
  glUniform3fv(glGetUniformLocation(id, name), 1, &data[0]);
}

void Shader::set_uniform(const char* name, glm::vec4 const& data) {
  glUniform4fv(glGetUniformLocation(id, name), 1, &data[0]);
}

void Shader::set_viewport(int w, int h, int x, int y) {
  glViewport(x, y, w, h);
}