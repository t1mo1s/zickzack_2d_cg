#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <iostream>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>

auto startTime = std::chrono::high_resolution_clock::now();
const double targetFPS = 120.0;
const double frameTime = 1.0 / targetFPS;

float xT = 0;

bool gameover = false;

glm::vec2 translate(glm::vec2 v, glm::vec2 trans) {
    // Erstelle eine 3x3 Translationsmatrix aus dem Übersetzungsvektor
    glm::mat3 translateMatrix = glm::mat3(
            1, 0, trans.x,
            0, 1, trans.y,
            0, 0, 1
    );

    // Füge eine homogene Koordinate hinzu und erstelle einen 3D-Vektor
    glm::vec3 vector3D = glm::vec3(v.x, v.y, 1.0f);

    // Multipliziere die Translationsmatrix mit dem 3D-Vektor mithilfe einer Schleife
    glm::vec3 translatedVector(0.0f);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            translatedVector[i] += translateMatrix[i][j] * vector3D[j];
        }
    }

    // Extrahiere die x- und y-Komponenten des übersetzten Vektors und gib sie als vec2 zurück
    return glm::vec2(translatedVector.x, translatedVector.y);
}

bool isPointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
    float detT = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / detT;
    float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / detT;
    float gamma = 1 - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}


class GameObject {
public:
    std::vector<float> verts;

    float changeL = 0.0f;
    // global variable for handling the vertex buffers of the game objects
    GLuint vertexbuffer;
    GLuint uvbuffer;
    float width, height;
    float x, y;
    //type of the game object
    int type;

    //virtual get verts
    virtual std::vector<float> getVerts() = 0;

    // Constructor
    GameObject(float x, float y){
        this->x = x;
        this->y = y;
    };

    virtual void draw() = 0;
    virtual void update() = 0;
    virtual bool initializeVAOs(){
        glGenVertexArrays(1, &uvbuffer);
        glBindVertexArray(uvbuffer);
        vertexbuffer_size = 6;

        GLfloat g_vertex_buffer_data[] = {
                1 , 0, 0,
                0, 1, 0,
                0,  0, 1,

                1,0,0,
                0,1,1,
                0,0,1

        };
        glGenBuffers(1, &this->vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        return true;
    };


    virtual bool cleanupVAOs(){
                glDeleteBuffers(1, &vertexbuffer);
                glDeleteVertexArrays(1, &uvbuffer);
                return true;
    };
};

class Player : public GameObject {
public:
    float xC, yC;


    Player(float x, float y) : GameObject(x, y) {
        this->x = x;
        this->y = y;
        this->xC = 0.5;
        this->yC = 0.75*1/6;
    }

    void update() override {
            GLfloat newVertexData[] = {
                    -1, -1, 0.0f,
                    1, -1, 0.0f,
                    1, 1, 0.0f,

                    1, 1.0f, 0.0f,
                    -1, 1.0f, 0.0f,
                    -1, -1, 0.0f,
            };
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexData), newVertexData);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::vector<float> getVerts(){
        return this->verts;
    }

    void draw() override{
        x = (xC-0.5)*2;
        y = ((yC/(768.0/1024.0))-0.5)*2;

        xC += xT ;

        GLuint isPlayerID = glGetUniformLocation(programID, "isPlayer");
        glUniform1i(isPlayerID, 1);

        // head the xC and yC to the fragment shader
        GLuint xCID = glGetUniformLocation(programID, "xC");
        glUniform1f(xCID, xC);
        GLuint yCID = glGetUniformLocation(programID, "yC");
        glUniform1f(yCID, yC);


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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

    };

    bool initializeVAOs() override {
            glGenVertexArrays(1, &uvbuffer);
            glBindVertexArray(uvbuffer);

            vertexbuffer_size = 6;
            GLfloat g_vertex_buffer_data[] = {
                    -0.35f , -0.5f, 0.0f,
                    0.35f, -0.5f, 0.0f,
                    0.35f,  0.5f, 0.0f,

                    0.35f,  0.5f, 0.0f,
                    -0.35f,  0.5f, 0.0f,
                    -0.35f, -0.5f, 0.0f,
            };

            glGenBuffers(1, &this->vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            return true;
        };
};

class Ground : public GameObject {

public:
    bool right = true;


    Ground(float x, float y, float fac, bool right) : GameObject(x, y) {

        this->changeL = fac;
        this->type = 1;
        this->x = x;
        this->y = y;
        this->right = right;
        //initialize the parallelogram
        verts.push_back(x);
        verts.push_back(y);

        verts.push_back(x + 0.2);
        verts.push_back(y);

        vec2 top_right;
        vec2 top_left;
        if (right == true) {
            top_right = translate(vec2(verts[2], verts[3]), vec2(0.5 * 0.75 * changeL, 0.5* changeL));
            top_left = translate(vec2(verts[0], verts[1]), vec2(0.5 * 0.75 * changeL, 0.5* changeL));
        } else {
            top_right = translate(vec2(verts[2], verts[3]), vec2(-0.5 * 0.75* changeL, 0.5* changeL));
            top_left = translate(vec2(verts[0], verts[1]), vec2(-0.5 * 0.75* changeL, 0.5* changeL));
        }

        verts.push_back(top_right.x);
        verts.push_back(top_right.y);

        verts.push_back(top_left.x);
        verts.push_back(top_left.y);

        this->height = verts[7];
        this->width = verts[6];
    }

    std::vector<float> getVerts(){
        return this->verts;
    }

    void update() override {

            GLfloat newVertexData[] = {
                    verts[0], verts[1], 0.0f,
                    verts[2], verts[3], 0.0f,
                    verts[4], verts[5], 0.0f,

                    verts[4], verts[5], 0.0f,
                    verts[6], verts[7], 0.0f,
                    verts[0], verts[1], 0.0f,
            };


        if (!gameover) {
            verts[1] -= 0.00025;
            verts[3] -= 0.00025;
            verts[5] -= 0.00025;
            verts[7] -= 0.00025;
        } else {
            verts[1] -= 0.0000;
            verts[3] -= 0.0000;
            verts[5] -= 0.0000;
            verts[7] -= 0.0000;
        }





        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexData), newVertexData);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw(){
        GLuint isPlayerID = glGetUniformLocation(programID, "isPlayer");
        glUniform1i(isPlayerID, 0);

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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

    };
};

class Start : public GameObject {
public:
    float xC, yC;

    Start(float x, float y) : GameObject(x, y) {
        this->x = x;
        this->y = y;
        this->xC = 0.5;
        this->yC = 0.1;

        //initialize the parallelogram
        verts.push_back(-1);
        verts.push_back(-1);

        verts.push_back(1);
        verts.push_back(-1);

        verts.push_back(1);
        verts.push_back(0);

        verts.push_back(-1);
        verts.push_back(0);

    }

    void update() override {
        GLfloat newVertexData[] = {
                verts[0], verts[1], 0.0f,
                verts[2], verts[3], 0.0f,
                verts[4], verts[5], 0.0f,

                verts[4], verts[5], 0.0f,
                verts[6], verts[7], 0.0f,
                verts[0], verts[1], 0.0f,

        };

        if (!gameover) {
            verts[1] -= 0.00025;
            verts[3] -= 0.00025;
            verts[5] -= 0.00025;
            verts[7] -= 0.00025;
        } else {
            verts[1] -= 0.0000;
            verts[3] -= 0.0000;
            verts[5] -= 0.0000;
            verts[7] -= 0.0000;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexData), newVertexData);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::vector<float> getVerts(){
        return this->verts;
    }

    void draw() override{

        xC += xT;

        GLuint isPlayerID = glGetUniformLocation(programID, "isPlayer");
        glUniform1i(isPlayerID, 0);

        // head the xC and yC to the fragment shader
        GLuint xCID = glGetUniformLocation(programID, "xC");
        glUniform1f(xCID, xC);
        GLuint yCID = glGetUniformLocation(programID, "yC");
        glUniform1f(yCID, yC);


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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

    };


    bool initializeVAOs() override {
        glGenVertexArrays(1, &uvbuffer);
        glBindVertexArray(uvbuffer);

        vertexbuffer_size = 6;
        GLfloat g_vertex_buffer_data[] = {
                -0.35f , -0.5f, 0.0f,
                0.35f, -0.5f, 0.0f,
                0.35f,  0.5f, 0.0f,

                0.35f,  0.5f, 0.0f,
                -0.35f,  0.5f, 0.0f,
                -0.35f, -0.5f, 0.0f,
        };

        glGenBuffers(1, &this->vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        return true;
    };
};

    std::shared_ptr<GameObject> Spieler = std::make_shared<Player>(0.0f, 0.0f);

    std::shared_ptr<GameObject> beg = std::make_shared<Start>(-1,-1);

    std::shared_ptr<GameObject> ground = std::make_shared<Ground>(-0.0, 0, 1,  true);

    std::shared_ptr<GameObject> groundleft = std::make_shared<Ground>(ground->width, ground->height, 2,false);

    std::shared_ptr<GameObject> ground2 = std::make_shared<Ground>(groundleft->width, groundleft->height, 0.2, true);

    std::shared_ptr<GameObject> ground3 = std::make_shared<Ground>(ground2->width, ground2->height, 0.2, false);

    std::shared_ptr<GameObject> ground4 = std::make_shared<Ground>(ground3->width, ground3->height, 0.2, true);

    std::shared_ptr<GameObject> ground5 = std::make_shared<Ground>(ground4->width, ground4->height, 0.2, false);

    std::shared_ptr<GameObject> ground6 = std::make_shared<Ground>(ground5->width, ground5->height, 0.2, true);

    std::shared_ptr<GameObject> ground7 = std::make_shared<Ground>(ground6->width, ground6->height, 0.2, false);

    std::shared_ptr<GameObject> ground8 = std::make_shared<Ground>(ground7->width, ground7->height, 3, true);

    std::vector<std::shared_ptr<GameObject>> gameObjects = { beg,ground, groundleft, ground2, ground3, ground4, ground5, ground6, ground7, ground8};

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //initialize the game objects
    Spieler->initializeVAOs();
    for (auto &gameObject : gameObjects) {
        gameObject->initializeVAOs();
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    do{
        updateAnimationLoop();
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    for (auto &gameObject : gameObjects) {
        gameObject->cleanupVAOs();
    }

  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}


void updateAnimationLoop()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // Use our shader
  glUseProgram(programID);

  bool ergebnis = true;

  for (auto &gameObject : gameObjects){
      bool col = !isPointInTriangle(glm::vec2(Spieler->x, Spieler->y),
                                      glm::vec2(gameObject->getVerts()[0], gameObject->getVerts()[1]),
                                      glm::vec2(gameObject->getVerts()[2], gameObject->getVerts()[3]),
                                      glm::vec2(gameObject->getVerts()[4], gameObject->getVerts()[5]))
                   &&
                   !isPointInTriangle(glm::vec2(Spieler->x, Spieler->y),
                                      glm::vec2(gameObject->getVerts()[4], gameObject->getVerts()[5]),
                                      glm::vec2(gameObject->getVerts()[6], gameObject->getVerts()[7]),
                                      glm::vec2(gameObject->getVerts()[0], gameObject->getVerts()[1])
                   );
        ergebnis = ergebnis && col;
  }

  if (ergebnis){
        cout << "Game Over\n";
        gameover = true;
  }

    auto currentTime = std::chrono::high_resolution_clock::now();
    double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - startTime).count();

    if (deltaTime < frameTime) {
        double sleepTime = frameTime - deltaTime;
        std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        deltaTime = frameTime;
    }
    //update and draw the game objects
    for (auto &gameObject : gameObjects) {
        gameObject->update();
        gameObject->draw();
    }
    Spieler->update();
    Spieler->draw();

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (!gameover) {
            xT = -0.00010;
        }
        else{
            xT = 0.0000;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (!gameover) {
            xT = 0.00010;
        }else{
            xT = 0.0000;
        }
    }

  glfwSwapBuffers(window);
  glfwPollEvents();

}



bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

