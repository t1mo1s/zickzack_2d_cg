#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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


float x = 0.0f;
float up = 0.0f;

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


class GameObject {
public:
    // global variable for handling the vertex buffers of the game objects
    GLuint vertexbuffer;
    GLuint uvbuffer;
    // for textures later
    GLuint textureSampleID;
    float width, height;
    float x, y;
    std::string type;
    bool isAlive = true;
    GLfloat matrix;
    GLuint vertexbuffer_size;
    glm::mat4 translateMatrix;

    // Constructor
    GameObject(float x, float y){
        this->x = x;
        this->y = y;
    };

    // pure virtual functions
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual bool initializeVAOs() = 0;
    virtual bool cleanupVAOs() = 0;
};

class layer : public GameObject {
public:
    layer(float x, float y) : GameObject(x, y) {
        this->x = x;
        this->y = y;
    }

    void draw() override {
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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // Swap buffers
    };

    void update() override {
            GLfloat newVertexData[] = {
                    -0.35f, -0.5f + up, 0.0f,
                    0.35f, -0.5f + up, 0.0f,
                    0.35f, 0.5f + up, 0.0f,
                    0.35f, 0.5f + up, 0.0f,
                    -0.35f, 0.5f + up, 0.0f,
                    -0.35f, -0.5f + up, 0.0f,
            };
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexData), newVertexData);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    bool initializeVAOs() override {
            glGenVertexArrays(1, &uvbuffer);
            glBindVertexArray(uvbuffer);

            vertexbuffer_size = 6;
            GLfloat g_vertex_buffer_data[] = {
                    -0.35f , -0.5f+up, 0.0f,
                    0.35f, -0.5f+up, 0.0f,
                    0.35f,  0.5f+up, 0.0f,

                    0.35f,  0.5f+up, 0.0f,
                    -0.35f,  0.5f+up, 0.0f,
                    -0.35f, -0.5f+up, 0.0f,
            };

            glGenBuffers(1, &this->vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            return true;
        };

        bool cleanupVAOs() override {
            glDeleteBuffers(1, &vertexbuffer);
            glDeleteVertexArrays(1, &uvbuffer);
            return true;
        };
};

class Ground : public GameObject {

public:
    std::vector<float> verts;

    Ground(float x, float y) : GameObject(x, y) {

        this->x = x;
        this->y = y;

        //initialize the parallelogram
        verts.push_back(x);
        verts.push_back(y);

        verts.push_back(x + 0.2);
        verts.push_back(y);

        vec2 top_right = translate(vec2(verts[2], verts[3]), vec2(0.5*0.75, 0.5));

        verts.push_back(top_right.x);
        verts.push_back(top_right.y);

        vec2 top_left = translate(vec2(verts[0], verts[1]), vec2(0.5*0.75, 0.5));

        verts.push_back(top_left.x);
        verts.push_back(top_left.y);


        this->height = verts[7];
        this->width = verts[6];
    }

    void draw() override {

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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // Swap buffers
    };

    void update() override {
        GLfloat newVertexData[] = {
                verts[0], verts[1], 0.0f,
                verts[2], verts[3], 0.0f,
                verts[4], verts[5], 0.0f,

                verts[4], verts[5], 0.0f,
                verts[6], verts[7], 0.0f,
                verts[0], verts[1], 0.0f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexData), newVertexData);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    bool initializeVAOs() override {
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

    bool cleanupVAOs() override {
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &uvbuffer);
        return true;
    };
};


class Groundleft : public GameObject {

public:
    std::vector<float> verts;
    Groundleft(float x, float y) : GameObject(x, y) {

        this->x = x;
        this->y = y;

        //initialize the parallelogram
        verts.push_back(x);
        verts.push_back(y);

        verts.push_back(x + 0.2);
        verts.push_back(y);

        float test = 2;

        vec2 top_right = translate(vec2(verts[2], verts[3]), vec2(-0.5*0.75*test, 0.5*test));

        verts.push_back(top_right.x);
        verts.push_back(top_right.y);

        vec2 top_left = translate(vec2(verts[0], verts[1]), vec2(-0.5*0.75*test, 0.5*test));

        verts.push_back(top_left.x);
        verts.push_back(top_left.y);


        this->height = verts[7];
        this->width = verts[6];
    }

    void draw() override {

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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // Swap buffers
    };

    void update() override {
        GLfloat newVertexData[] = {
                verts[0], verts[1], 0.0f,
                verts[2], verts[3], 0.0f,
                verts[4], verts[5], 0.0f,

                verts[4], verts[5], 0.0f,
                verts[6], verts[7], 0.0f,
                verts[0], verts[1], 0.0f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertexData), newVertexData);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    bool initializeVAOs() override {
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

    bool cleanupVAOs() override {
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &uvbuffer);
        return true;
    };
};

    //create a player
    std::shared_ptr<GameObject> Player = std::make_shared<layer>(0.0f, 0.0f);

    //create a ground
    std::shared_ptr<GameObject> ground = std::make_shared<Ground>(-0.25, -1);

    //create a groundleft
    std::shared_ptr<GameObject> groundleft = std::make_shared<Groundleft>(ground->width, ground->height);

    //create a groundright
    std::shared_ptr<GameObject> ground2 = std::make_shared<Ground>(groundleft->width, groundleft->height);

    // list of all game objects
    std::vector<std::shared_ptr<GameObject>> gameObjects = {Player, ground, groundleft, ground2};
//create a groundle
int main( void )
{


    // standard matrix
    glm::mat4 identity = glm::mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
            );

    auto startTime = std::chrono::high_resolution_clock::now();
    const double targetFPS = 120.0;
    const double frameTime = 1.0 / targetFPS;

  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;


    //initialize the game objects
    for (auto &gameObject : gameObjects) {
        gameObject->initializeVAOs();
    }

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


  // Get a handle for our "MVP" uniform
    GLuint translationXID = glGetUniformLocation(programID, "translationX");
    GLuint translationYID = glGetUniformLocation(programID, "translationY");

	//start animation loop until escape key is pressed
	do{

        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - startTime).count();

        if (deltaTime < frameTime) {
            double sleepTime = frameTime - deltaTime;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            deltaTime = frameTime;  // Reset deltaTime after sleep
        }


        glClear(GL_COLOR_BUFFER_BIT);


        //update and draw the game objects
        for (auto &gameObject : gameObjects) {
            gameObject->update();
            gameObject->draw();
        }





        //initializeVertexbuffer();
        //updateAnimationLoop();
        // do everything neccessary to draw the player

        if (glfwGetKey(window, GLFW_KEY_W)) {
            up += 0.01f;
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  Player->cleanupVAOs();
  cleanupVertexbuffer();
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

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);

  // Swap buffers
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
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  return true;
}

bool initializeVertexbuffer()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    vertexbuffer_size = 6;
    GLfloat g_vertex_buffer_data[] = {
            -0.35f , -0.5f+up, 0.0f,
            0.35f, -0.5f+up, 0.0f,
            0.35f,  0.5f+up, 0.0f,

            0.35f,  0.5f+up, 0.0f,
            -0.35f,  0.5f+up, 0.0f,
            -0.35f, -0.5f+up, 0.0f,
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    return true;
}


bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

