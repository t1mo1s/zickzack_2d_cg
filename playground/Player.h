//
// Created by pfaff on 10/31/2023.
//

#ifndef OPENGL_TEMPLATE_PLAYER_H
#define OPENGL_TEMPLATE_PLAYER_H
#include "AbstractGameObject.h"


class Player : public AbstractGameObject {
private:
    int radius;

public:
    Player(float x, float y, float width, float height, std::string type);

    //move
    std::vector<float> draw() override;

};


#endif //OPENGL_TEMPLATE_PLAYER_H
