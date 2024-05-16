#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shader.h"

/* Callback to resize viewport on window resize */
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Init GLAD */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  /* Set viewport and register resize callback */
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  // glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  /* Create shader */
  unsigned int shader = newShader("./shaders/shader.vs", "./shaders/shader.fs");

  /* ** VERTEX DATA  ** */
  float vertices[] = {
      // Positions        //Colours
       0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
      -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
  };
  unsigned int indices[] = {
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

  /* Setup vertex array object */
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  /* Create vertex buffer object and bind and load it into memory */
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Setup element array object to hold indices */
  unsigned int EAO;
  glGenBuffers(1, &EAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  /* Position attribute */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  /* Color attribute */
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  /* Unbind VBO and VAO */

  /* You can unbind the VBO, the call to glVertexAttribPointer registered VBO as the
   * vertex attribute's bound vertex buffer object so afterwards we can safely unbind */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  /* DON'T unbind element buffer obj as it will remove the bind inside the VAO */
  glBindVertexArray(0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw */
    useShader(shader);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
