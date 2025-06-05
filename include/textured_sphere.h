#ifndef TEXTURED_SPHERE_H
#define TEXTURED_SPHERE_H

#include "sphere.h"
#include "texture.h"

class TexturedSphere : public Sphere {
public:
    TexturedSphere(Shader *shader_program, Texture *texture, bool isSun);
    void setShadowed(bool s) { shadowed_ = s; }
    void setShadowFactor(float factor);
    //float shadow_factor_ = 1.0f; 
    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

    GLuint getID() const { return textureID; }

private:
    GLuint loc_diffuse_map;
    Texture *texture;
    // Permet de récupérer l'ID de la texture
    GLuint textureID;
    Shader* shader;
    bool isSun_;
    bool shadowed_ = false;
	float shadow_factor_ = 1.0f; // Facteur d'ombre pour l'illumination
};

#endif // TEXTURED_SPHERE_H
