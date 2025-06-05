#include "textured_sphere.h"

TexturedSphere::TexturedSphere(Shader* shader_program, Texture* texture, bool isSun)
	: Sphere(shader_program), texture(texture), isSun_(isSun) 
{
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

void TexturedSphere::setShadowFactor(float factor) {
    shadow_factor_ = factor;
}

void TexturedSphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);

    glUniform3f(glGetUniformLocation(this->shader_program_, "lightPos"), 0.0f, 0.0f, -3.0f);
    glUniform3f(glGetUniformLocation(this->shader_program_, "viewPos"), 0.0f, 8.0f, 12.0f);

    glUniform1i(glGetUniformLocation(this->shader_program_, "isSun"), isSun_ ? 1 : 0);
    glUniform1i(glGetUniformLocation(this->shader_program_, "isShadowed"), shadowed_ ? 1 : 0);
    glUniform1f(glGetUniformLocation(this->shader_program_, "shadowFactor"), shadow_factor_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getGLid());
    glUniform1i(loc_diffuse_map, 0);

    Sphere::draw(model, view, projection);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}