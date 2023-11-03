//
// Created by pfaff on 10/31/2023.
//

#include "Player.h"

#include <glm/glm.hpp>
#include <vector>
#include <glfw3.h>


using namespace glm;

Player::Player(float x, float y, float width, float height, std::string type) : AbstractGameObject(x, y, width, height, type) {
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->type=type;
}

void Player::draw(){

    // normalized aspect ratio
    /*
    float nAR = xRes / yRes;
    float size = 0.2f;
    glm::mat4 mv = glm::mat4(
            -1 * size, 0, 0, x,
            0, nAR * -1 * size, 0, y,
            0, 0, 1, 0,
            0, 0, 0, 1);


    GLfloat matrix = glGetUniformLocation(programID, "mv");
    glUniformMatrix4fv(matrix, 1, GL_FALSE, &mv[0][0]);


    // Use our shader
    glUseProgram(programID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureSamplerID, 0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle
     */
}

void Player::update() {

}

void Player::initializeVAOs() {

}

void Player::cleanupVAOs() {

}

//void Player::checkCollision() {

