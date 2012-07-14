#include "shader.h"
#include "define.h"
#include "tool.h"
#include <iostream>
#include <cassert>

#ifndef WINDOWS

Shader::Shader() : m_valid(false)
{
}

Shader::~Shader()
{
}


bool Shader::Load(const std::string& key, const OptionList& options)
{
    return LoadFromFile(key + ".vert", key + ".frag", options.GetBoolean("verbose", true));
}

bool Shader::Release()
{
    // TODO cleanup here?
	return true;
}

bool Shader::LoadFromFile(const std::string& vertFile, const std::string& fragFile, bool verbose)
{
    std::string fragmentShader;
    std::string vertexShader;

    if(verbose)
        std::cout << "Loading vertex shader (" << vertFile << "..." << std::endl;
    if(!Tool::LoadTextFile(vertFile, vertexShader))
    {
        if(verbose)
            std::cout << "Failed to load " << vertFile << std::endl;
        return false;
    }

    if(verbose)
        std::cout << "Loading fragment shader (" << fragFile << "..." << std::endl;
    if(!Tool::LoadTextFile(fragFile, fragmentShader))
    {
        if(verbose)
            std::cout << "Failed to load " << fragFile << std::endl;
        return false;
    }

    return LoadFromMemory(vertexShader, fragmentShader, verbose);
}

bool Shader::LoadFromMemory(const std::string& vertShader, const std::string& fragShader, bool verbose)
{
    const char* my_vertex_shader_source = vertShader.c_str();
    const char* my_fragment_shader_source = fragShader.c_str();

    //std::cout << fragmentShader << std::endl;
    //std::cout << vertexShader << std::endl;

    m_program = glCreateProgram();
    CHECK_GL_ERROR();
    assert(glIsProgram(m_program));

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER_ARB);
    CHECK_GL_ERROR();
    assert(glIsShader(m_vertexShader));

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER_ARB);
    CHECK_GL_ERROR();
    assert(glIsShader(m_fragmentShader));

    // Load Shader Sources
    glShaderSource(m_vertexShader, 1, (const GLchar**)&my_vertex_shader_source, NULL);
    CHECK_GL_ERROR();
    glShaderSource(m_fragmentShader, 1, (const GLchar**)&my_fragment_shader_source, NULL);
    CHECK_GL_ERROR();

    // Compile The Shaders
    if(verbose)
        std::cout << "Compiling vertex shader..." << std::endl;
    glCompileShader(m_vertexShader);
    if(!CheckShaderError(m_vertexShader, verbose))
        return false;

    if(verbose)
        std::cout << "Compiling fragment shader..." << std::endl;
    glCompileShader(m_fragmentShader);
    if(!CheckShaderError(m_fragmentShader, verbose))
        return false;

    // Attach The Shader Objects To The Program Object
    glAttachShader(m_program, m_vertexShader);
    CHECK_GL_ERROR();
    glAttachShader(m_program, m_fragmentShader);
    CHECK_GL_ERROR();

    // Link The Program Object
    glLinkProgram(m_program);
    //if(!CheckProgramError(m_program, verbose))
    //    return false;
    CheckProgramError(m_program, true, verbose);
    CHECK_GL_ERROR();

    m_valid = true;
    return true;
}

void Shader::Use() const
{
    assert(m_valid);
    glUseProgram(m_program);
}

bool Shader::IsValid() const
{
    return m_valid;
}

GLint Shader::BindIntUniform(const std::string& name) const
{
    assert(m_valid);
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::UpdateIntUniform(GLint name, GLint value) const
{
    assert(m_valid);
    glUniform1i(name, value);
}

void Shader::SetIntUniform(const std::string& name, GLint value) const
{
    assert(m_valid);
    GLint location = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetVec3Uniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
{
    assert(m_valid);
    GLint location = glGetUniformLocation(m_program, name.c_str());
    glUniform3f(location, x, y, z);
}

void Shader::SetFloatUniform(const std::string& name, GLfloat value) const
{
    assert(m_valid);
    GLint location = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(location, value);
}

void Shader::UpdateFloatUniform(GLint name, GLfloat value) const
{
    assert(m_valid);
    glUniform1f(name, value);
}

void Shader::SetMat4Uniform(const std::string& name, float values[16]) const
{
    assert(m_valid);
    GLint location = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(location,  1, false, values);
}

void Shader::Disable()
{
    glUseProgram(0);
}

bool Shader::CheckShaderError(GLenum shader, bool verbose)
{
    GLint compileOk;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk);
    if(verbose && !compileOk)
    {
        int maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

        std::cout << infoLog << std::endl;
        delete [] infoLog;
        return false;
    }

    return compileOk;
}

bool Shader::CheckProgramError(GLenum program, bool showWarning, bool verbose)
{
    GLint compileOk;

    glGetProgramiv(program, GL_LINK_STATUS, &compileOk);
    CHECK_GL_ERROR();
    if(verbose && (showWarning || !compileOk))
    {
        int maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        CHECK_GL_ERROR();

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);
        CHECK_GL_ERROR();

        std::cout << infoLog << std::endl;
        delete [] infoLog;
    }

    return compileOk;
}

#endif
