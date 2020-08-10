#include "glad/glad.h"
#include "window.h"

Window::Window(int x, int y, const char* name)
    : mdelta()
    , mpos()
    , cursor(0)
    , time(0)
    , dt(0)
{
    if (!glfwInit()) {
        printf("Failed to create window\n");
        exit(-1);
    }
    win = glfwCreateWindow(x, y, name, 0, 0);
    glfwMakeContextCurrent(win);
    glfwSetInputMode(win, GLFW_RAW_MOUSE_MOTION, glfwRawMouseMotionSupported());
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGL()) {
        printf("Failed to init GL\n");
        exit(-1);
    }
    glClearColor(0.14, .32, 0.54, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glfwSwapInterval(0);
}

Window::~Window() {
    glfwDestroyWindow(win);
}

bool Window::poll()
{
    glfwPollEvents();
    glfwSwapBuffers(win);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(win, 1);
    }
    double x, y;
    glfwGetCursorPos(win, &x, &y);
    glm::vec2 tmp = { x, y };
    mdelta = tmp - mpos;
    mpos = tmp;
    {
        float tmp = glfwGetTime();
        dt = tmp - time;
        time = tmp;
    }

    return !glfwWindowShouldClose(win);
}


bool Window::get_key(int keycode) const
{
    return glfwGetKey(win, keycode);
}

void Window::toggle_mouse(int state)
{
    glfwSetInputMode(win, GLFW_CURSOR, state);
}

void Window::get_size(int& w, int& h)
{
    glfwGetWindowSize(win, &w, &h);
}
