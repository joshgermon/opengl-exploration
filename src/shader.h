#ifndef SHADER_H
#define SHADER_H

#include "../include/glad/glad.h"

unsigned int newShader(const char* vertexPath, const char* fragmentPath);
void useShader(unsigned int id);
void shaderSetBool(unsigned int id, const char *name, int value);
void shaderSetInt(unsigned int id, const char *name, int value);
void shaderSetFloat(unsigned int id, const char *name, float value);

#endif
