#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords; // on va simuler les coords comme avant

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    TexCoords = vec2(
        atan(position.z, position.x) / (2.0 * 3.1415926) + 0.5,
        asin(position.y) / 3.1415926 + 0.5
    );
}
