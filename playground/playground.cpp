#include "playground.h"

// Include standard headers
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "AbstractGameObject.h"
#include "Player.h"

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

int main( void )
{
    static float x = 0;
    static float y = 0;

  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//start animation loop until escape key is pressed
	do{

        std::this_thread::sleep_for(std::chrono::milliseconds(50));


        initializeVertexbuffer();
        updateAnimationLoop(x, y);

        x += 0.01;
        y += 0.02;


	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void drawCircle(float centerX, float centerY, float radius, int segments)
{
    // Kreis zeichnen mit dem Mittelpunkt (centerX, centerY) und Radius radius
    std::vector<float> circleVertices;
    float angleIncrement = 2 * M_PI / segments;

    for (int i = 0; i < segments; ++i) {
        float angle = i * angleIncrement;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        circleVertices.push_back(x);
        circleVertices.push_back(y);
        circleVertices.push_back(0.01f); // Z-Koordinate (0.0f für 2D)
    }

    GLuint circleVertexBuffer;
    glGenBuffers(1, &circleVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, circleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * circleVertices.size(), circleVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, circleVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Zeichne den Kreis
    glDrawArrays(GL_TRIANGLE_FAN, 0, circleVertices.size() / 3);

    glDisableVertexAttribArray(0);

    // Lösche den Vertex Buffer nach dem Zeichnen des Kreises
    glDeleteBuffers(1, &circleVertexBuffer);
}

void updateAnimationLoop(float x, float y)
{

    // sleep for 1 second
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  drawCircle(0.0f+x, 0.0f+y, 0.5f, 100);
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
  //glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

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

  vertexbuffer_size = 3;
  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
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

