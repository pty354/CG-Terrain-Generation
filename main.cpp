#include <windows.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <iostream>

#include "GenerateTerrain.h"
#include "Controller.h"
#include "FreeImage.h"
#include "Shader.h"

using namespace std;


Terrain* terrain;
Controller* controller;
Camera* camera;

static const string vertex_shader("shaders/shader.vert");
static const string fragment_shader("shaders/shader.frag");

static const string grassFile = "textures/Grass.tga";
static const string rockFile = "textures/Rock.tga";
static const string rock2File = "textures/Rock2.tga";
static const string snowFile = "textures/Snow.tga";
static const string mudFile = "textures/Mud.tga";

GLuint grassTexture = 0;
GLuint rockTexture = 0;
GLuint rock2Texture = 0;
GLuint snowTexture = 0;
GLuint mudTexture = 0;
GLuint vao = 0;
GLuint shader = 0;

float ratio = 0.67f;

vec3 lightDir(-1, -1, -1);
bool useLight = true;

void LoadShader()
{
    GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

    if (new_shader == 0) // loading failed
    {
        glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    }
    else
    {
        glClearColor(0.0f, 0.47f, 1.0f, 0.0f);

        if (shader != 0)
        {
            glDeleteProgram(shader);
        }
        shader = new_shader;

    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);


    glm::mat4 matrix = glm::rotate(0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, rockTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, rock2Texture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, snowTexture);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mudTexture);

    int texGrass_loc = glGetUniformLocation(shader, "grass");
    if (texGrass_loc != 0)
    {
        glUniform1i(texGrass_loc, 0); // we bound our texture to texture unit 0
    }
    int texRock_loc = glGetUniformLocation(shader, "rock");
    if (texRock_loc != 0)
    {
        glUniform1i(texRock_loc, 1); // we bound our texture to texture unit 0
    }
    int texRock2_loc = glGetUniformLocation(shader, "rock2");
    if (texRock2_loc != 0)
    {
        glUniform1i(texRock2_loc, 2); // we bound our texture to texture unit 0
    }
    int texSnow_loc = glGetUniformLocation(shader, "snow");
    if (texSnow_loc != 0)
    {
        glUniform1i(texSnow_loc, 3); // we bound our texture to texture unit 0
    }
    int texMud_loc = glGetUniformLocation(shader, "mud");
    if (texMud_loc != 0)
    {
        glUniform1i(texMud_loc, 4); // we bound our texture to texture unit 0
    }

    int light_loc = glGetUniformLocation(shader, "light");
    if (light_loc != 0) {
        glUniform3fv(light_loc, 1, &lightDir[0]);
    }

    camera->upload(shader);
    int M_loc = glGetUniformLocation(shader, "M");
    if (M_loc != -1)
    {
        glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(matrix));
    }
    int useLight_loc = glGetUniformLocation(shader, "useLight");
    if (useLight_loc) {
        glUniform1f(useLight_loc, useLight);
    }
    glUniform1f(glGetUniformLocation(shader, "hMin"), terrain->hMin);
    glUniform1f(glGetUniformLocation(shader, "hMax"), terrain->hMax);

    if (vao != 0)
        DrawTerrain(vao, terrain->N);

    controller->update();

    glutSwapBuffers();
}

//load textures using freeimage library 
GLuint LoadTexture(const char* fname)
{
    GLuint tex_id;

    FIBITMAP* tempImg = FreeImage_Load(FreeImage_GetFileType(fname, 0), fname);
    FIBITMAP* img = FreeImage_ConvertTo32Bits(tempImg);

    FreeImage_Unload(tempImg);

    GLuint w = FreeImage_GetWidth(img);
    GLuint h = FreeImage_GetHeight(img);
    GLuint scanW = FreeImage_GetPitch(img);

    GLubyte* byteImg = new GLubyte[h * scanW];
    FreeImage_ConvertToRawBits(byteImg, img, scanW, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
    FreeImage_Unload(img);

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, byteImg);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete byteImg;

    return tex_id;
}

void initOpenGl()
{
    //Initialize glew so that new OpenGL function names can be used

    glewInit();
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_PRIMITIVE_RESTART);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    LoadShader();
    grassTexture = LoadTexture(grassFile.c_str());
    rockTexture = LoadTexture(rockFile.c_str());
    rock2Texture = LoadTexture(rock2File.c_str());
    snowTexture = LoadTexture(snowFile.c_str());
    mudTexture = LoadTexture(mudFile.c_str());
    terrain = new Terrain(ratio);
    vao = create_terrain_vao(&terrain->v, terrain->N);

}


void keyboard(unsigned char key, int x, int y)
{
    controller->keyboard(key, x, y);
    switch (key)
    {
    case 'n':
        delete terrain;
        terrain = new Terrain(ratio);
        vao = create_terrain_vao(&terrain->v, terrain->N);
        break;
    default:
        break;
    }
}

void keyboard_up(unsigned char key, int x, int y)
{
    controller->keyboard_up(key, x, y);
}

void idle()
{
    glFlush();
    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(1280, 720);
    int win = glutCreateWindow("Procedural Terrain Generation");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);

    glutIdleFunc(idle);
    initOpenGl();
    camera = new Camera(glm::vec3(0, 5, 5));
    controller = new Controller(camera, 1280, 720);

    glutMainLoop();
    glutDestroyWindow(win);
    return 0;
}