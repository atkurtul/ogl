#pragma once
#include "GLFW/glfw3.h"
#include "common.h"


struct Window {
  GLFWwindow* win;

  glm::vec2 mdelta;
  glm::vec2 mpos;

  bool cursor;

  float time;
  float dt;
  Window(const char* name);

  ~Window();

  bool poll();

  bool get_key(int keycode) const;

  void toggle_mouse(int state);

  void get_size(int& w, int& h);
};