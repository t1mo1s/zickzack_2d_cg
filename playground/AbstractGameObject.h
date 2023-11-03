#ifndef OPENGL_TEMPLATE_ABSTRACTGAMEOBJECT_H
#define OPENGL_TEMPLATE_ABSTRACTGAMEOBJECT_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>


class AbstractGameObject {
public:
    // global variable for handling the vertex buffers of the game objects
    GLuint vertexbuffer;
    GLuint uvbuffer;
    // for textures later
    GLuint textureSampleID;
    GLuint texture;
    float width, height;
    float x, y;
    std::string type;
    bool isAlive = true;
    GLfloat matrix;
    GLuint MatrixID;
    GLuint vertexbuffer_size;
    glm::mat4 translateMatrix;
    float speed;
    float radius;
    bool isMoving;

    // Constructor
    AbstractGameObject(float x, float y, float width, float height, std::string type);

    // pure virtual functions
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void initializeVAOs() = 0;
    virtual void cleanupVAOs() = 0;

    // collision detection
    //virtual void checkCollision() = 0;

    // Setter und Getter für isAlive
    bool IsAlive() const;

    // Getter und Setter für x, y, width, height
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
};

#endif //OPENGL_TEMPLATE_ABSTRACTGAMEOBJECT_H
