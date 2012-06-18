#ifndef TAK__SHADER_H
#define TAK__SHADER_H
#include <string>
#include "define.h"
#include "nocopy.h"

class Shader
{
    NOCOPY(Shader);

    public:    
        Shader();
        ~Shader();

        bool LoadFromFile(const std::string& vertFile, const std::string& fragFile, bool verbose = false);
        bool LoadFromMemory(const std::string& vertShader, const std::string& fragShader, bool verbose = false);
        void Use() const;

        bool IsValid() const;

        GLint BindIntUniform(const std::string& name) const;
        void UpdateIntUniform(GLint name, GLint value) const;
        void SetIntUniform(const std::string& name, GLint value) const;
        void SetVec3Uniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
        void SetFloatUniform(const std::string& name, GLfloat value) const;
        void UpdateFloatUniform(GLint name, GLfloat value) const;
        void SetMat4Uniform(const std::string& name, float values[16]) const;

        static void Disable();

    private:
        GLenum m_program;
        GLenum m_vertexShader;
        GLenum m_fragmentShader;

        bool m_valid;

    private:
        bool CheckShaderError(GLenum shader, bool verbose);
        bool CheckProgramError(GLenum program, bool showWarning, bool verbose);
};


#endif // SHADER_H
