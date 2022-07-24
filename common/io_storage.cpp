#include "io_storage.h"

std::shared_ptr<IOInterface>& IOStorage::request(short id) {
    return storage[id];
}

void IOStorage::put(const std::shared_ptr<IOInterface>& io) {
    storage[io->id] = io;
}

void IOStorage::remove(short id) {
    storage.erase(id);
}

void IOStorage::remove(const std::shared_ptr<IOInterface>& io) {
    auto it = storage.begin();
    while (it != storage.end()) {
        if (it->second == io) {
            storage.erase(it);
            return;
        } 
        it++;
    }
}

std::unordered_map<short, std::shared_ptr<IOInterface>> IOStorage::storage;