#include <glad/glad.h>
#include "window.h"
#include "../../common/thread_safety.h"
#include <iostream>
#include <algorithm>
#include <utility>

GLWindow::GLWindow(int w, int h, std::string title)
    : width(w), height(h), title(std::move(title)), renderer(nullptr), surface(nullptr), isHeadless(false) {
    element = new PositionedElement(0.f, 0.f, (float) w, (float) h);
}

GLWindow::~GLWindow() {
    destroy();
    if (surface) {
        delete surface;
        surface = nullptr;
    }
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
}

bool GLWindow::create() {
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (isHeadless) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!handle) {
        std::cout << "Unable to create window" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(handle);
    gladLoadGL();
    glViewport(0, 0, width, height);
    return true;
}

void GLWindow::destroy() {
    glfwDestroyWindow(handle);
}

GLFWwindow* GLWindow::getHandle() {
    return handle;
}

int GLWindow::getWidth() const {
    return width;
}

int GLWindow::getHeight() const {
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
    if (surface && renderer) {
        surface->render(renderer);
    }
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

PositionedElement* GLWindow::getElement() {
    return element;
}

void GLWindow::headless() {
    isHeadless = true;
    if (handle) {
        glfwHideWindow(handle);
    }
}

void GLWindow::addInputHandler(IInputHandler* handler) {
    inputHandlers.push_back(handler);
}

void GLWindow::removeInputHandler(IInputHandler* handler) {
    inputHandlers.erase(std::find(inputHandlers.begin(), inputHandlers.end(), handler));
}

void GLWindow::removeInputHandler(int idx) {
    removeInputHandler(inputHandlers[idx]);
}

std::vector<IInputHandler*> GLWindow::getInputHandlers() {
    return inputHandlers;
}