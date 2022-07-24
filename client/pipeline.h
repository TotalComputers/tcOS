#pragma once

#include "handler.h"
#include <string>
#include <unordered_map>
#include <deque>
#include <functional>

using handler_callback = std::function<bool(AbstractHandler*, int)>;

class Pipeline {
public:
    void addLast(const std::string&, AbstractHandler*);
    void addFirst(const std::string&, AbstractHandler*);
    void addAfter(const std::string&, const std::string&, AbstractHandler*);
    void addBefore(const std::string&, const std::string&, AbstractHandler*);
    void insert(int, const std::string&, AbstractHandler*);
    void remove(const std::string&);
    int indexOf(const std::string&);
    void replace(const std::string&, AbstractHandler*);
    void forEach(const handler_callback&, int, int);
    void forEach(const handler_callback&, int);
    void forEach(const handler_callback&);
    AbstractHandler* get(int);
    AbstractHandler* get(const std::string&);
    std::unordered_map<std::string, AbstractHandler*>& handlers();

private:
    std::unordered_map<std::string, AbstractHandler*> pipeline;
    std::deque<std::string> order;

};