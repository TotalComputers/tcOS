#pragma once

#include "io.h"
#include <memory>

class IOFactory {
public:
    virtual std::shared_ptr<IOInterface> create() = 0;

};