//
// Created by pfaff on 10/31/2023.
//

#include "Player.h"


Player::Player(float x, float y, float width, float height, std::string type) : AbstractGameObject(x, y, width, height, type) {
    this->hello = 0;
}

int Player::getHello() const {
    return hello;
}

void Player::draw() {
    std::cout << "Player moved" << std::endl;
}
