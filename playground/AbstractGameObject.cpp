//
// Created by pfaff on 10/30/2023.
//

#include "AbstractGameObject.h"
// Include GLFW
#include <glfw3.h>


AbstractGameObject::AbstractGameObject(float x, float y, float width, float height, std::string type) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->type = type;
}

bool AbstractGameObject::IsAlive() const {
    return this->isAlive;
}

float AbstractGameObject::getX() const {
    return this->x;
}

float AbstractGameObject::getY() const {
    return this->y;
}

float AbstractGameObject::getWidth() const {
    return this->width;
}

float AbstractGameObject::getHeight() const {
    return this->height;
}

//void AbstractGameObject::checkCollision() {
//}




