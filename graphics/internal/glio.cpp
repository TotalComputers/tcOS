#include "glio.h"
#include "../../common/thread_safety.h"
#include <iostream>
#include <mutex>
#include <algorithm>

std::mutex gl_mutex;

GLIO::GLIO(IWindowFactory* factory)
    : factory(factory) {}

void GLIO::init() {
    gl_mutex.lock();
    run_in_main_thread([&]() {
        gl = factory->createWindow(reference_buf.width, reference_buf.height, name);
    });
    gl_mutex.unlock();
    inputHandlers = gl->getInputHandlers();
    free(reference_buf.data);
    reference_buf.data = 0;
}

void GLIO::set_frame(image_t frame)  {
    reference_buf = frame;
}

void GLIO::destroy() {
    gl_mutex.lock();
    run_in_main_thread([&]() {
        delete gl;
    });
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

void GLIO::handle_touch(int x, int y, bool is_right, bool admin) {
    for(IInputHandler* handler : inputHandlers) {
        handler->handleTouch(x, y, is_right, admin);
    }
}

void GLIO::addInputHandler(IInputHandler* handler) {
    inputHandlers.push_back(handler);
}

void GLIO::removeInputHandler(IInputHandler* handler) {
    inputHandlers.erase(std::find(inputHandlers.begin(), inputHandlers.end(), handler));
}

void GLIO::removeInputHandler(int idx) {
    removeInputHandler(inputHandlers[idx]);
}


GLIOFactory::GLIOFactory(IWindowFactory* factory)
    : factory(factory) {}

std::shared_ptr<IOInterface> GLIOFactory::create() {
    return std::make_shared<GLIO>(factory);
}