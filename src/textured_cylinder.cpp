#include "textured_cylinder.h"

TexturedCylinder::TexturedCylinder(Shader* shader_program, Texture* texture)
    : Cylinder(shader_program), texture(texture)
{
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

void TexturedCylinder::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {

    glUseProgram(this->shader_program_);

    glActiveTexture(GL_TEXTURE0); // Activer l'unité de texture 0
    glBindTexture(GL_TEXTURE_2D, texture->getGLid()); // Lier la texture

    glUniform1i(loc_diffuse_map, 0); // 0 correspond à GL_TEXTURE0

    Cylinder::draw(model, view, projection);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}
