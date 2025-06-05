#pragma once

#include "sphere.h"
#include "texture.h"
#include <glm/glm.hpp>

class TexturedLightingSphere : public Sphere {
public:
    TexturedLightingSphere(Shader* shader_program, Texture* texture,
                            glm::vec3 light_position, glm::vec3 light_color);

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Texture* texture;
    glm::vec3 light_position;
    glm::vec3 light_color;
    GLuint loc_light_pos;
    GLuint loc_light_color;
    GLuint loc_diffuse_map;
};
