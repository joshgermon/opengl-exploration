#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"

char *readFile(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: Could not open file %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, length, file);
    if (bytesRead != length) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: Could not read file %s\n", filePath);
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

void checkCompileErrors(unsigned int shader, char *type)
{
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM")) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}

unsigned int newShader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    char *vertexCode = readFile(vertexPath);
    char *fragmentCode = readFile(fragmentPath);

    if (!vertexCode || !fragmentCode) return 0;

    // 2. compile shaders
    unsigned int shaderId, vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char **)&vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char **)&fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    shaderId = glCreateProgram();
    glAttachShader(shaderId, vertex);
    glAttachShader(shaderId, fragment);
    glLinkProgram(shaderId);
    checkCompileErrors(shaderId, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // Don't forget to free the allocated memory
    free(vertexCode);
    free(fragmentCode);

    return shaderId;
}

void useShader(unsigned int id) {
    glUseProgram(id);
}

// utility uniform functions
// ------------------------------------------------------------------------
void shaderSetBool(unsigned int id, const char *name, int value) {
    glUniform1i(glGetUniformLocation(id, name), value);
}
// ------------------------------------------------------------------------
void shaderSetInt(unsigned int id, const char *name, int value) {
    glUniform1i(glGetUniformLocation(id, name), value);
}
// ------------------------------------------------------------------------
void shaderSetFloat(unsigned int id, const char *name, float value) {
    glUniform1f(glGetUniformLocation(id, name), value);
}
