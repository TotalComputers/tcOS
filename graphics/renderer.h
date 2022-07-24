#pragma once

class IRenderer {
public:
    virtual ~IRenderer() = default;

public: 
    virtual void render() = 0;

};
