//
// Created by pfaff on 10/30/2023.
//

#include "AbstractGameObject.h"


AbstractGameObject::AbstractGameObject(float x, float y, float width, float height, std::string type) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->type = type;
    this->isAlive = true;
}

void AbstractGameObject::setType(const std::string &type) {
    AbstractGameObject::type = type;
}

std::string AbstractGameObject::getType() const {
    return type;
}

void AbstractGameObject::setIsAlive(bool alive) {
    isAlive = alive;
}

bool AbstractGameObject::getIsAlive() const {
    return isAlive;
}

float AbstractGameObject::getX() const {
    return x;
}

void AbstractGameObject::setX(float x) {
    AbstractGameObject::x = x;
}

float AbstractGameObject::getY() const {
    return y;
}

void AbstractGameObject::setY(float y) {
    AbstractGameObject::y = y;
}

float AbstractGameObject::getWidth() const {
    return width;
}

float AbstractGameObject::getHeight() const {
    return height;
}



