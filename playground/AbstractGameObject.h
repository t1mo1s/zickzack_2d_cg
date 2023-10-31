#ifndef OPENGL_TEMPLATE_ABSTRACTGAMEOBJECT_H
#define OPENGL_TEMPLATE_ABSTRACTGAMEOBJECT_H

#include <iostream>

class AbstractGameObject {
private:
    std::string type;
    bool isAlive;
    float x;
    float y;
    float width;
    float height;

public:
    AbstractGameObject(float x, float y, float width, float height, std::string type);

    // Setter und Getter für type
    void setType(const std::string& type);
    std::string getType() const;

    // Setter und Getter für isAlive
    void setIsAlive(bool alive);
    bool getIsAlive() const;

    // Getter und Setter für x, y, width, height
    float getX() const;
    void setX(float x);
    float getY() const;
    void setY(float y);
    float getWidth() const;
    float getHeight() const;
};

#endif //OPENGL_TEMPLATE_ABSTRACTGAMEOBJECT_H
