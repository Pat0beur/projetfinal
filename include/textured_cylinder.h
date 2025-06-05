#ifndef TEXTURED_CYLINDER_H
#define TEXTURED_CYLINDER_H

#include "cylinder.h"
#include "texture.h"

class TexturedCylinder : public Cylinder {
public:
    TexturedCylinder(Shader* shader_program, Texture* texture);

    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

    GLuint getID() const { return textureID; }

private:
    GLuint loc_diffuse_map;
    Texture* texture;
    // Permet de récupérer l'ID de la texture
    GLuint textureID;
    Shader* shader;
};

#endif // TEXTURED_CYLINDER_H