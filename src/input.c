#include "../include/glad/glad.h"
#include "cglm/cam.h"
#include "shader.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>

float viewX = 0.0f;
float viewY = 0.0f;
float viewZ = -3.0f;

/* Callback to resize viewport on window resize */
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    viewX -= 0.1f;

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    viewX += 0.1f;

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    viewY -= 0.1f;

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    viewY += 0.1f;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    if(viewZ > -3.0) viewZ -= 0.1f;

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    viewZ += 0.1f;
}

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  /* Create shader */
  unsigned int shader = newShader("./shaders/shader.vs", "./shaders/shader.fs");

  /* ** VERTEX DATA  ** */
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  /* Setup vertex array object */
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  /* Create vertex buffer object and bind and load it into memory */
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Position attribute */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  /* Texture attribute */
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* === Unbind VBO and VAO */
  /* You can unbind the VBO, the call to glVertexAttribPointer registered VBO as
   * the vertex attribute's bound vertex buffer object so afterwards we can
   * safely unbind */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  /* DON'T unbind element buffer obj as it will remove the bind inside the VAO
   */
  glBindVertexArray(0);

  int txW, txH, nrChannels;
  stbi_set_flip_vertically_on_load(1);
  unsigned char *data =
      stbi_load("./assets/container.jpg", &txW, &txH, &nrChannels, 0);

  if (!data)
    printf("Failed to load texture");

  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txW, txH, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  data = stbi_load("./assets/awesomeface.png", &txW, &txH, &nrChannels, 0);
  if (!data)
    printf("Failed to load texture");

  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txW, txH, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  // tell opengl for each sampler to which texture unit it belongs to (only has
  // to be done once)
  useShader(shader);
  shaderSetInt(shader, "texture1", 0);
  shaderSetInt(shader, "texture2", 1);

  // Setup all our CUBES
    vec3 viewPositions[] = {
        {0.0f,  0.0f,  0.0f},
        {2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        {2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        {1.3f, -2.0f, -2.5f},
        {1.5f,  2.0f, -2.5f},
        {1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    /* Render here */
    glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    useShader(shader);

    /* Create view matrix */
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){viewX, viewY, viewZ});
    /* Create projection matrix */
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(45.0), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    int viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);

    int projLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)projection);

    glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    for(unsigned int i =0; i < 10; i++) {
      mat4 model = GLM_MAT4_IDENTITY_INIT;
      glm_translate(model, viewPositions[i]);
      float angle = 20.0f * (i + 1);
      glm_rotate(model, glfwGetTime() * glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});

      int modelLoc = glGetUniformLocation(shader, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
