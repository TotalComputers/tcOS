#pragma once

#include "../ui/element.h"
#include "../../input/input_handler.h"
#include "../surface.h"
#include <GLFW/glfw3.h>
#include <string>

class GLWindow {
public:
    GLWindow(int, int, std::string = "");
    ~GLWindow();
    void headless();
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
    PositionedElement* getElement();
    IRenderer* getRenderer();
    void addInputHandler(IInputHandler*);
    void removeInputHandler(IInputHandler*);
    void removeInputHandler(int);
    std::vector<IInputHandler*> getInputHandlers();

private:
    GLFWwindow* handle;
    int width, height;
    std::string title;
    ISurface* surface;
    IRenderer* renderer;
    PositionedElement* element;
    bool isHeadless;
    std::vector<IInputHandler*> inputHandlers;

};