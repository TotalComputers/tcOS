#include "glio.h"
#include "../../common/thread_safety.h"
#include <iostream>

GLIO::GLIO(IWindowFactory* factory)
    : factory(factory) {}

void GLIO::init() {
    run_in_main_thread([&]() {
        gl = factory->createWindow(reference_buf.width, reference_buf.height, name);
    });
    free(reference_buf.data);
    reference_buf.data = 0;
}

void GLIO::set_frame(image_t frame)  {
    reference_buf = frame;
}

void GLIO::destroy() {
    run_in_main_thread([&]() {
        gl->destroy();
    });
    delete gl;
    gl = nullptr;
}

image_t& GLIO::provide_frame() {
    if(!gl) return reference_buf;
    run_in_main_thread([&]() {
        gl->doLoopWork();
    });
    return ((PBOSurface*)gl->getSurface())->buffer;
}

void GLIO::handle_touch(int, int, bool, bool) {
    std::cout << "stub" << std::endl;
}


GLIOFactory::GLIOFactory(IWindowFactory* factory)
    : factory(factory) {}

std::shared_ptr<IOInterface> GLIOFactory::create() {
    return std::make_shared<GLIO>(factory);
}