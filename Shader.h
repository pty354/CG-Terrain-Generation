#ifndef __INITSHADER_H__
#define __INITSHADER_H__

#include <windows.h>
//#include <GL/GL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <fstream>
#include <iostream>
GLuint InitShader(const char* vertexShaderFile, const char* fragmentShaderFile);

#endif
