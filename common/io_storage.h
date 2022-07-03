#pragma once

#include "io.h"
#include <unordered_map>
#include <memory>

class IOStorage {
public:
    static std::shared_ptr<IOInterface> request(short);
    static void put(std::shared_ptr<IOInterface>);
    static void remove(short);
    static void remove(std::shared_ptr<IOInterface>);

private:
    static std::unordered_map<short, std::shared_ptr<IOInterface>> storage;

};