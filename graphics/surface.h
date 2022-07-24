#pragma once

#include "renderer.h"
#include <vector>

class ISurface {
public:
    virtual ~ISurface();

public:
    virtual void render(IRenderer*) = 0;

};

class CombinedSurface : public ISurface {
public:
    ~CombinedSurface() override;

public:
    int addSurface(ISurface*);
    ISurface* getSurface(int);

public:
    void render(IRenderer*) override;

private:
    std::vector<ISurface*> surfaces;

};

class NativeSurface : public ISurface {
public:
    ~NativeSurface() override;

public:
    void render(IRenderer*) override;

};