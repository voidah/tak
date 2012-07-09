#include "tool.h"
#include "define.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <ctype.h>

void Tool::CheckTypes()
{
    // Make sure defined types are of the good size
    // Useful if we compile for a different compiler/architecture
    // and type size aren't changed
    assert(sizeof(uint8) == 1);
    assert(sizeof(uint16) == 2);
    assert(sizeof(uint32) == 4);
    assert(sizeof(uint64) == 8);
    assert(sizeof(int8) == 1);
    assert(sizeof(int16) == 2);
    assert(sizeof(int32) == 4);
    assert(sizeof(int64) == 8);
}

bool Tool::LoadTextFile(const std::string& filename, std::string& buffer)
{
    std::ifstream f(filename.c_str(), std::ios::binary);
    if(!f.is_open())
        return false;

    f.seekg(0, std::ios::end);
    unsigned int len = f.tellg();
    f.seekg(0, std::ios::beg);

    char* tmp = new char[len + 1];

    f.read(tmp, len);
    f.close();

    tmp[len] = 0;
    buffer = tmp;
    delete [] tmp;

    return true;
}

void Tool::CheckGLError(const char* file, int line)
{
    GLuint err = glGetError();
    if (err != GL_NO_ERROR)	
    {
        std::cerr << "Opengl error before " << file << "[" << line << "]:" << std::hex << err << "(";

        switch(err)
        {
            case GL_INVALID_ENUM:
                std::cerr << "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                std::cerr << "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                std::cerr << "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                std::cerr << "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr << "GL_OUT_OF_MEMORY";
                break;
            case GL_TABLE_TOO_LARGE:
                std::cerr << "GL_TABLE_TOO_LARGE";
                break;
            default:
                std::cerr << "unknown";
        }
        std::cerr << ")" << std::endl;
        std::cerr << "ATTENTION: this error might come from anywhere in the code since the previous call to CHECK_GL_ERROR" << std::endl;
        exit(1);
    }
}

unsigned int Tool::SplitString(const std::string &input, const std::string &delim, std::vector<std::string> &output)
{
    static const std::string::size_type npos = std::string::size_type(-1);

    std::string::size_type begin=0, end;
    unsigned int count=0;

    if(input.length() == 0 || delim.length() == 0)
        return 0;

    while((end = input.find(delim, begin)) != npos)
    {
        output.push_back(input.substr(begin, end - begin));
        ++count;
        begin = end + delim.length();
    }

    // add the last token:
    output.push_back(input.substr(begin, input.length() - begin));

    return ++count;    
}

std::string Tool::Trim(const std::string& str)
{
    std::string result;
    std::string::size_type pos1 = 0, pos2 = str.length();

    if(pos2 == 0)
        return str;

    while(pos1 < str.size() && (str[pos1] == ' ' || str[pos1] == '\t' || str[pos1] == '\r' || str[pos1] == '\n'))
        ++pos1;

    pos2--;
    while(pos2 > 0 && (str[pos2] == ' ' || str[pos2] == '\t' || str[pos2] == '\r' || str[pos2] == '\n'))
        --pos2;

    return str.substr(pos1, pos2 - pos1 + 1);
}

std::string Tool::ToLower(std::string str)
{
    for(int i = 0; i < str.length(); ++i)
        str[i] = (char)tolower(str[i]);

    return str;
}

std::string Tool::ToUpper(std::string str)
{
    for(int i = 0; i < str.length(); ++i)
        str[i] = (char)toupper(str[i]);

    return str;
}

