#include "viewer.h"
#include "skybox.h"
#include "node.h"
#include "texture.h"
#include "textured_sphere.h"

#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

// Retourne true si 'occluder' est entre 'planet' et le Soleil
bool is_in_shadow(const glm::vec3& planet_pos, float planet_radius,
    const glm::vec3& occluder_pos, float occluder_radius,
    const glm::vec3& sun_pos)
{
    glm::vec3 sun_to_planet = glm::normalize(planet_pos - sun_pos);
    glm::vec3 sun_to_occluder = glm::normalize(occluder_pos - sun_pos);

    float alignment = glm::dot(sun_to_planet, sun_to_occluder);
    if (alignment < 0.999f) // seuil d'alignement, ajuste si besoin
        return false;

    float dist_sun_occluder = glm::length(occluder_pos - sun_pos);
    float dist_sun_planet = glm::length(planet_pos - sun_pos);
    if (dist_sun_occluder > dist_sun_planet)
        return false;

    glm::vec3 proj = sun_pos + sun_to_planet * dist_sun_occluder;
    float dist_center = glm::length(proj - occluder_pos);
    if (dist_center < occluder_radius)
        return true;

    return false;
}

Viewer::Viewer(int width, int height)
{
    skybox = nullptr;
    if (!glfwInit())    // initialize window system glfw
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // version hints: create GL window with >= OpenGL 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    // make win's OpenGL context current; no OpenGL calls can happen before
    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set user pointer for GLFW window to this Viewer instance
    glfwSetWindowUserPointer(win, this);

    // register event handlers
    glfwSetKeyCallback(win, key_callback_static);

    // useful message to check OpenGL renderer characteristics
    std::cout << glGetString(GL_VERSION) << ", GLSL "
        << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
        << glGetString(GL_RENDERER) << std::endl;

    // initialize GL by setting viewport and default render characteristics
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer
    than anything already drawn at that pixel */
    glEnable(GL_DEPTH_TEST); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
    glDepthFunc(GL_LESS);

    // initialize our scene_root
    scene_root = new Node();
}

void Viewer::run()
{
    double start_time = glfwGetTime();

    while (!glfwWindowShouldClose(win))
    {
        // clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);

        // Caméra plongeante : en hauteur et en recul
        glm::vec3 camera_pos = glm::vec3(0.0f, 8.0f, 20.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, -3.0f); // vise le centre du Soleil
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // axe vertical

        glm::mat4 view = glm::lookAt(camera_pos, target, up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 60.0f);
        if (SunView) {
            glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, -3.0f);
            glm::vec3 front;
            //Calcul du vecteur de direction de la caméra sur le soleil en coordonnées sphériques
            front.x = cos(glm::radians(sunViewAngles.y)) * cos(glm::radians(sunViewAngles.x));
            front.y = sin(glm::radians(sunViewAngles.x));
            front.z = sin(glm::radians(sunViewAngles.y)) * cos(glm::radians(sunViewAngles.x));
            view = glm::lookAt(sunPos, sunPos + glm::normalize(front), glm::vec3(0.0f, 1.0f, 0.0f));
            scene_root->remove(Soleil_node);
        }
        else {
            // Vue par défaut (plongeante)
            view = glm::lookAt(
                glm::vec3(0.0f, 8.0f, 20.0f),
                glm::vec3(0.0f, 0.0f, -3.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        if (skybox) {
            skybox->draw(view, projection);
        }
        update();
        scene_root->draw(model, view, projection);

        // Poll for and process events
        glfwPollEvents();

        // flush render commands, and swap draw buffers
        glfwSwapBuffers(win);
    }
    /* close GL context and any other GLFW resources */
    glfwTerminate();
}

void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key,action); // Ajout de l'action pour l'appui unique
}

void Viewer::on_key(int key,int action)
{
    // 'Q' or 'Escape' quits
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
    //Permet d'avoir un appui unique
    if (key == GLFW_KEY_ENTER) {
        if (action == GLFW_PRESS) {
            enterKeyPressed = true;
        }
        else if (action == GLFW_RELEASE) {
            if (enterKeyPressed) {
                SunView = !SunView;
                enterKeyPressed = false;
            }
        }
    }
    else if (SunView && key == GLFW_KEY_LEFT) {
        sunViewAngles.y -= 2.0f; 
    }
    else if (SunView && key == GLFW_KEY_RIGHT) {
        sunViewAngles.y += 2.0f;
    }
    else if (SunView && key == GLFW_KEY_UP) {
        sunViewAngles.x -= 2.0f;
    }
    else if (SunView && key == GLFW_KEY_DOWN) {
        sunViewAngles.x += 2.0f;
    }
}

void Viewer::update() {
    double time = glfwGetTime(); // temps global

    struct OrbitInfo {
        Node* node;
        float orbit_radius;      // distance au Soleil
        float orbit_speed;       // vitesse de révolution (degrés/seconde)
        float scale;             // taille de la planète
        float self_rotation;     // vitesse de rotation sur elle-même (optionnel)
        float y_offset;          // décalage vertical (pour Jupiter par ex)
    };

    // Soleil au centre, ne tourne pas autour de lui-même
    if (Soleil_node != nullptr) {
            if (SunView) {
                // Retirer temporairement le Soleil de la scène
                scene_root->remove(Soleil_node);
            }
            else {
                // Vérifier d'abord qu'il n'est pas déjà présent
                bool isPresent = false;
                for (auto child : scene_root->getChildren()) {
                    if (child == Soleil_node) {
                        isPresent = true;
                        break;
                    }
                }
                //Vérifie si le soleil est présent dans la scène
                if (!isPresent) {
                    scene_root->add(Soleil_node);
                }
                glm::mat4 mat =
                    glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians((float)time * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * // rotation sur lui-même
                    glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
                Soleil_node->set_transform(mat);
            }
            //Initialisation des plnaètes
            std::vector<OrbitInfo> planets = {
                {Mercure_node, 1.5f,  60.0f, 0.3f,   0.0f, 0.0f},
                {Venus_node,   2.2f,  45.0f, 0.4f,   0.0f, 0.0f},
                {Terre_node,   3.0f,  30.0f, 0.5f,  90.0f, 0.0f},
                {Mars_node,    3.8f,  24.0f, 0.4f,   0.0f, 0.0f},
                {Jupiter_node, 5.0f,  10.0f, 0.7f,  90.0f, 0.0f},
                {Saturne_node, 6.2f,  12.0f, 0.6f,   0.0f, 0.0f},
                {Uranus_node,  7.4f,  15.0f, 0.5f,   0.0f, 0.0f},
                {Neptune_node, 8.6f,   8.0f, 0.5f,   0.0f, 0.0f}
            };

            // Position du Soleil
            glm::vec3 sun_pos = glm::vec3(0.0f, 0.0f, -3.0f);

            // Stocke les états des planètes
            struct PlanetState {
                Node* node;
                glm::vec3 pos;
                float radius;
            };
            std::vector<PlanetState> planet_states;

            // 1. Calcule la position de chaque planète et stocke les infos
            for (const auto& planet : planets) {
                if (planet.node != nullptr) {
                    glm::mat4 mat =
                        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, planet.y_offset, -3.0f)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians((float)time * planet.orbit_speed), glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::translate(glm::mat4(1.0f), glm::vec3(planet.orbit_radius, 0.0f, 0.0f)) *
                        glm::scale(glm::mat4(1.0f), glm::vec3(planet.scale)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(planet.self_rotation + (float)time * 60.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    planet.node->set_transform(mat);

                    glm::vec3 pos = glm::vec3(mat * glm::vec4(0, 0, 0, 1));
                    planet_states.push_back({ planet.node, pos, planet.scale * 0.5f });
                }
            }

            // 2. Détermine si chaque planète est dans l'ombre d'une autre
            for (size_t i = 0; i < planet_states.size(); ++i) {
                bool shadowed = false;
                float dist_sun_planet = glm::length(planet_states[i].pos - sun_pos);

                for (size_t j = 0; j < planet_states.size(); ++j) {
                    if (i == j) continue;
                    float dist_sun_occluder = glm::length(planet_states[j].pos - sun_pos);

                    // Seules les planètes plus proches du Soleil peuvent faire de l'ombre
                    if (dist_sun_occluder < dist_sun_planet) {
                        if (is_in_shadow(planet_states[i].pos, planet_states[i].radius,
                            planet_states[j].pos, planet_states[j].radius,
                            sun_pos)) {
                            shadowed = true;
                            break;
                        }
                    }
                }
                auto* ts = dynamic_cast<TexturedSphere*>(planet_states[i].node->get_children_shape()[0]);
                if (ts) {
                    float shadow_factor = shadowed ? 0.4f : 1.0f; // 0.4 = ombre douce, ajuste selon l'effet désiré
                    ts->setShadowFactor(shadow_factor);
                }
            }
        }
    }