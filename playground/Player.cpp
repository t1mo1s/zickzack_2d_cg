//
// Created by pfaff on 10/31/2023.
//

#include "Player.h"

#include <glm/glm.hpp>
#include <vector>
#include <glfw3.h>


using namespace glm;


Player::Player(float x, float y, float width, float height, std::string type) : AbstractGameObject(x, y, width, height, type) {

}


std::vector<float> Player::draw() {
    // Kreis zeichnen mit dem Mittelpunkt (centerX, centerY) und Radius radius
    int segments = 100;

    std::vector<float> circleVertices;
    float angleIncrement = 2 * M_PI / segments;

    for (int i = 0; i < segments; ++i) {
        float angle = i * angleIncrement;
        float x = this->getX() + radius * std::cos(angle);
        float y = this->getY() + radius * std::sin(angle);
        circleVertices.push_back(x);
        circleVertices.push_back(y);
        circleVertices.push_back(0.0f); // Z-Koordinate (0.0f für 2D)
    }
}
