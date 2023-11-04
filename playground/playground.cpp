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
    GLuint MatrixID;
    GLuint vertexbuffer_size;
    glm::mat4 translateMatrix;
    float speed;
    float radius;
    bool isMoving;

    // Constructor
    GameObject(float x, float y, float width, float height, std::string type){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->type = type;
    };

    // pure virtual functions
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual bool initializeVAOs() = 0;
    virtual bool cleanupVAOs() = 0;

    // collision detection
    //virtual void checkCollision() = 0;

    // Setter und Getter für isAlive
    bool IsAlive() const{
        return this->isAlive;
    };

    // Getter und Setter für x, y, width, height
    float getX() const{
        return this->x;
    };
    float getY() const{
        return this->y;
    };
    float getWidth() const {
        return this->width;
    };
    float getHeight() const{
        return this->height;
    };
};

class layer : public GameObject {
public:
    layer(float x, float y, float width, float height, std::string type) : GameObject(x, y, width, height, type) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->type = type;
    }

    void draw() override {

        this->y = up;

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

    //create a player
    std::shared_ptr<GameObject> Player = std::make_shared<layer>(0.0f, 0.0f, 0.5f, 0.5f, "player");

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

  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;



    Player->initializeVAOs();
  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


	//start animation loop until escape key is pressed
	do{
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - startTime;
        startTime = currentTime;

        // Calculate the movement based on the elapsed time and desired speed (e.g., 0.1 units per second)
        float speed = 0.5f; // adjust the speed as needed
        float deltaTime = elapsedTime.count();
        float movement = speed * deltaTime;

        Player->update();
        Player->draw();


        //initializeVertexbuffer();
        //updateAnimationLoop();
        // do everything neccessary to draw the player

        if (glfwGetKey(window, GLFW_KEY_W)) {
            up += 0.01f;
        }

        // print the x and y coordinates
        std::cout << "x: " << x << " y: " << up << std::endl;


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

