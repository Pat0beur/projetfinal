#version 330 core

in vec2 frag_tex_coords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuse_map;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool isSun;
uniform float shadowFactor;

out vec4 FragColor;

void main() {
    vec3 texColor = texture(diffuse_map, frag_tex_coords).rgb;

    if (isSun) {
        FragColor = vec4(texColor, 1.0);
    } else {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = 0.2 * texColor;
        vec3 diffuse = diff * texColor;
        vec3 specular = 0.5 * spec * vec3(1.0);

        vec3 result = ambient + diffuse + specular;
        // Applique le facteur d'ombre ici
        FragColor = vec4(result * shadowFactor, 1.0);
    }
}