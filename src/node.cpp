#include "node.h"
#include "shape.h"
#include <iostream>

Node::Node(const glm::mat4& transform) :
    transform_(transform) {

        children_ = std::vector<Node*>();
}

void Node::add(Node* node) {
    children_.push_back(node);
}

void Node::add(Shape* shape) {
    children_shape_.push_back(shape);
}

// Ajout de la fonction remove pour pouvoir retirer le Soleil
void Node::remove(Node* node) {
    auto it = std::find(children_.begin(), children_.end(), node);
    if (it != children_.end()) {
        children_.erase(it);
    }
}
// Pour récupérer ce qu'il y a dans la scène
std::vector<Node*> Node::getChildren() const {
    return children_;
}

void Node::set_transform(const glm::mat4& new_transform) {
    this->transform_ = new_transform;
}

void Node::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glm::mat4 updatedModel = model * transform_;

    for (auto child : children_) {
        child->draw(updatedModel, view, projection);
    }

    for (auto child : children_shape_) {
            child->draw(updatedModel, view, projection);
    }
}

void Node::key_handler(int key) const {
    for (const auto& child : children_) {
            child->key_handler(key);
    }
}
