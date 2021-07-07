#include "Shader.h"


using namespace std;

static char* readShaderSource(const char* shaderFile)
{
    ifstream ifs(shaderFile, ios::in | ios::binary | ios::ate);
    if (ifs.is_open())
    {
        unsigned int filesize = static_cast<unsigned int>(ifs.tellg());
        ifs.seekg(0, ios::beg);
        char* bytes = new char[filesize + 1];
        memset(bytes, 0, filesize + 1);
        ifs.read(bytes, filesize);
        ifs.close();
        return bytes;
    }
    return NULL;
}

void printShaderCompileError(GLuint shader)
{
    GLint  logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char* logMsg = new char[logSize];
    glGetShaderInfoLog(shader, logSize, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;

}

void printProgramLinkError(GLuint program)
{
    GLint  logSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    char* logMsg = new char[logSize];
    glGetProgramInfoLog(program, logSize, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;
}

GLuint InitShader(const char* vShaderFile, const char* fShaderFile)
{

    bool error = false;
    struct Shader
    {
        const char* filename;
        GLenum       type;
        GLchar* source;
    }  shaders[2] =
    {
       { vShaderFile, GL_VERTEX_SHADER, NULL },
       { fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };

    GLuint program = glCreateProgram();

    for (int i = 0; i < 2; ++i)
    {
        Shader& s = shaders[i];
        s.source = readShaderSource(s.filename);
        if (shaders[i].source == NULL)
        {
            std::cerr << "Failed to read " << s.filename << std::endl;
            error = true;
        }

        GLuint shader = glCreateShader(s.type);
        glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
        glCompileShader(shader);

        GLint  compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            std::cerr << s.filename << " failed to compile:" << std::endl;
            printShaderCompileError(shader);
            error = true;
        }

        delete[] s.source;

        glAttachShader(program, shader);
    }

    /* link  and error check */
    glLinkProgram(program);

    GLint  linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        std::cerr << "Shader program failed to link" << std::endl;
        printProgramLinkError(program);

        error = true;
    }

    if (error == true)
    {
        return -1;
    }

    /* use program object */
    glUseProgram(program);
    return program;
}