#include <glad/glad.h>
#include "window.h"
#include <iostream>

bool glfwInitialized = false;

GLWindow::GLWindow(int w, int h, std::string title)
    : width(w), height(h), title(title), renderer(nullptr), surface(nullptr) {}

bool GLWindow::create() {
    if(!glfwInitialized) {
        glfwInit();
        glfwInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if(!handle) {
        std::cout << "Unable to create window" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(handle);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Unable to initialize glad" << std::endl;
        return false;
    }
    glViewport(0, 0, width, height);
    return true;
}

void GLWindow::destroy() {
    glfwDestroyWindow(handle);
}

GLFWwindow* GLWindow::getHandle() {
    return handle;
}

int GLWindow::getWidth() {
    return width;
}

int GLWindow::getHeight() {
    return height;
}

std::string GLWindow::getTitle() {
    return title;
}

bool GLWindow::shouldClose() {
    return glfwWindowShouldClose(handle);
}

void GLWindow::doLoopWork() {
    makeCurrent();
    if(surface && renderer)
        surface->render(renderer);
    glfwPollEvents();
    glfwSwapBuffers(handle);
}

void GLWindow::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::makeCurrent() {
    glfwMakeContextCurrent(handle);
    glViewport(0, 0, width, height);
}

void GLWindow::setSurface(ISurface* surface) {
    this->surface = surface;
}

ISurface* GLWindow::getSurface() {
    return surface;
}

void GLWindow::setRenderer(IRenderer* renderer) {
    this->renderer = renderer;
}

IRenderer* GLWindow::getRenderer() {
    return renderer;
}