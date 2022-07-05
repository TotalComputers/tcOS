#include "glio.h"
#include "../../common/thread_safety.h"
#include <iostream>
#include <mutex>

std::mutex gl_mutex;

GLIO::GLIO(IWindowFactory* factory)
    : factory(factory) {}

void GLIO::init() {
    gl_mutex.lock();
    run_in_main_thread([&]() {
        gl = factory->createWindow(reference_buf.width, reference_buf.height, name);
    });
    gl_mutex.unlock();
    free(reference_buf.data);
    reference_buf.data = 0;
}

void GLIO::set_frame(image_t frame)  {
    reference_buf = frame;
}

void GLIO::destroy() {
    gl_mutex.lock();
    run_in_main_thread([&]() {
        gl->destroy();
    });
    delete gl;
    gl = nullptr;
    gl_mutex.unlock();
}

image_t& GLIO::provide_frame() {
    if(!gl) return reference_buf;
    gl_mutex.lock();
    run_in_main_thread([&]() {
        gl->doLoopWork();
    });
    gl_mutex.unlock();
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