//
// Created by pfaff on 10/31/2023.
//

#ifndef OPENGL_TEMPLATE_PLAYER_H
#define OPENGL_TEMPLATE_PLAYER_H
#include "AbstractGameObject.h"


class Player : public AbstractGameObject {
public:
    Player(float x, float y, float width, float height, std::string type);

    void draw() override;

    void update() override;

    void initializeVAOs() override;

    void cleanupVAOs() override;

    //void checkCollision() override;
};


#endif //OPENGL_TEMPLATE_PLAYER_H
