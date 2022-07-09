#pragma once

#include "../../common/io.h"
#include "../../common/io_factory.h"
#include "../../input/input_handler.h"
#include "../pbo_surface.h"
#include "window_factory.h"
#include <iostream>

class GLIO : public IOInterface {
public:
    GLIO(IWindowFactory*);

public:
    void init() override;
    void destroy() override;
    image_t& provide_frame();
    void set_frame(image_t) override;
    void handle_touch(int, int, bool, bool) override;
    void addInputHandler(IInputHandler*);
    void removeInputHandler(IInputHandler*);
    void removeInputHandler(int);

private:
    IWindowFactory* factory;
    image_t reference_buf;
    GLWindow* gl;
    std::vector<IInputHandler*> inputHandlers;

};

class GLIOFactory : public IOFactory {
public:
    GLIOFactory(IWindowFactory*);

public:
    std::shared_ptr<IOInterface> create() override;

private:
    IWindowFactory* factory;

};