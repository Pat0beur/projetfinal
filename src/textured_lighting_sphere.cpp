#include "textured_lighting_sphere.h"
#include <glm/gtc/type_ptr.hpp>

TexturedLightingSphere::TexturedLightingSphere(Shader* shader_program, Texture* texture,
                                               glm::vec3 light_position, glm::vec3 light_color)
    : Sphere(shader_program), texture(texture), light_position(light_position), light_color(light_color)
{
    loc_light_pos = glGetUniformLocation(this->shader_program_, "lightPos");
    loc_light_color = glGetUniformLocation(this->shader_program_, "lightColor");
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

void TexturedLightingSphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(this->shader_program_);

    glUniform3fv(loc_light_pos, 1, glm::value_ptr(light_position));
    glUniform3fv(loc_light_color, 1, glm::value_ptr(light_color));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getGLid());
    glUniform1i(loc_diffuse_map, 0); // GL_TEXTURE0 = 0

    Sphere::draw(model, view, projection);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}
