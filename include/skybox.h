#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp> 

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint cubemapTexture;
    GLuint VAO, VBO;
    GLuint shader_program; 

    GLuint loadCubemap(const std::vector<std::string>& faces);
    GLuint loadSkyboxShader(const std::string& vertex_path, const std::string& fragment_path);
};
