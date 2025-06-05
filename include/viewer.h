#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "node.h"
#include "skybox.h"

class Viewer {
public:
    Viewer(int width = 640, int height = 480);
    void update();
    void run();
    void on_key(int key, int action);
    Skybox* skybox;
    Node* scene_root;
    GLFWwindow* win;
    Node* Terre_node = nullptr;
    Node* Mars_node = nullptr;
    Node* Soleil_node = nullptr;
    Node* Saturne_node = nullptr;
    Node* Jupiter_node = nullptr;
    Node* Mercure_node = nullptr;
    Node* Venus_node = nullptr;
    Node* Neptune_node = nullptr;
    Node* Uranus_node = nullptr;

private:
    glm::vec2 sunViewAngles = glm::vec2(0.0f);  // Stocke les angles de rotation
    bool SunView = false;  // false = vue normale
    bool enterKeyPressed = false;  // Pour détecter l'appui
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif // VIEWER_H
