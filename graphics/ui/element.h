#pragma once

#include "../internal/shader.h"
#include "../internal/vao.h"
#include "../internal/vbo.h"
#include <vector>
#include <map>

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
typedef void(__stdcall* glDeleteFn)(int, const unsigned int*); 
#else
typedef void(*glDeleteFn)(int, const unsigned int*);
#endif

class PositionedElement {
public:
    PositionedElement(float, float, float, float);
    virtual void parentChanged(PositionedElement*);
    virtual void childChanged(PositionedElement*);
    virtual void thisChanged();
    void setParent(PositionedElement*);
    PositionedElement* getParent();
    std::vector<PositionedElement*> getChildren();
    void addChild(PositionedElement*);
    void removeChild(PositionedElement*);
    void removeChild(int);
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    float getLeft();
    float getTop();
    float getRight();
    float getBottom();
    void setX(float);
    void setY(float);
    void setWidth(float);
    void setHeight(float);
    void set(float, float, float, float);
    glm::mat4 ortho();
    glm::mat4 model();

private:
    void notifyChanges();

private:
    float x, y, w, h;
    PositionedElement* parent;
    std::vector<PositionedElement*> children;

};

class IRenderableElement : public PositionedElement {
public:
    IRenderableElement(float, float, float, float);
    virtual void display() = 0;

};

class CachedElement : public IRenderableElement {
public:
    CachedElement(float, float, float, float, unsigned char = 4);
    ~CachedElement();

public:
    virtual void parentChanged(PositionedElement*) override;
    virtual void thisChanged() override;
    virtual void render(int) = 0;
    void setDisplayShader(Shader*, const std::string&);
    Shader* getDisplayShader();
    void cache(int);
    void recache(int);
    void display() override;
    void bindLayer(int, int);
    int getSamples();
    void setSamples(unsigned char);

private:
    std::vector<unsigned int> layers;
    std::vector<int> binding;
    std::map<unsigned int, glDeleteFn> deletable;
    Shader* displayShader;
    unsigned char numSamples;
    glm::mat4 matrix;
    unsigned int matrixLoc;

private:
    static VAO* vao;
    static VBO* vbo;
    static VAO* getRenderVAO();

};