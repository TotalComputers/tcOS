#pragma once

#include "../surface.h"
#include <GLFW/glfw3.h>
#include <string>

class GLWindow {
public:
    GLWindow(int, int, std::string = "");
    ~GLWindow();
    bool create();
    void destroy();
    GLFWwindow* getHandle();
    int getWidth();
    int getHeight();
    std::string getTitle();
    bool shouldClose();
    void doLoopWork();
    static void clear(float, float, float, float);
    void makeCurrent();
    void setSurface(ISurface*);
    ISurface* getSurface();
    void setRenderer(IRenderer*);
    IRenderer* getRenderer();

private:
    GLFWwindow* handle;
    int width, height;
    std::string title;
    ISurface* surface;
    IRenderer* renderer;

};