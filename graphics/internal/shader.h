#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader {
public: 
    Shader();
    ~Shader();

public:
    void setVertexFile(const std::string&);
    void setFragmentFile(const std::string&);
    void setVertexSource(const std::string&);
    void setFragmentSource(const std::string&);
    [[nodiscard]] unsigned int getHandle() const;
    void create();
    void bind() const;
    static void unbind();

    static void uniformBool (int, bool);
    static void uniformInt  (int, int);
    static void uniformFloat(int, float); 
    static void uniformVec2f(int, const glm::vec2&);
    static void uniformVec3f(int, const glm::vec3&);
    static void uniformVec4f(int, const glm::vec4&);
    static void uniformMat2f(int, const glm::mat2&);
    static void uniformMat3f(int, const glm::mat3&);
    static void uniformMat4f(int, const glm::mat4&);

    void uniformBool (const std::string&, bool);
    void uniformInt  (const std::string&, int);
    void uniformFloat(const std::string&, float); 
    void uniformVec2f(const std::string&, const glm::vec2&);
    void uniformVec3f(const std::string&, const glm::vec3&);
    void uniformVec4f(const std::string&, const glm::vec4&);
    void uniformMat2f(const std::string&, const glm::mat2&);
    void uniformMat3f(const std::string&, const glm::mat3&);
    void uniformMat4f(const std::string&, const glm::mat4&);

    unsigned int uniformLocation(const std::string&) const;

private:
    std::string vertexSource, fragmentSource;
    unsigned int handle = 0;

};