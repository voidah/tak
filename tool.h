#ifndef TAK__TOOL_H__
#define TAK__TOOL_H__

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>

// TODO ne pas oublier de ne pas definir DEBUGMODE en release
#ifndef DEBUGMODE
#define DEBUGMODE
#endif

#ifdef DEBUGMODE
#	define CHECK_GL_ERROR() Tool::CheckGLError(__FILE__, __LINE__);
#else
#	define CHECK_GL_ERROR()
#endif


class Tool
{
    public:
        static void CheckTypes();
        static bool LoadTextFile(const std::string& filename, std::string& buffer);
        static void CheckGLError(const char* file, int line);

        template <class T>
            static bool EqualWithEpsilon(const T& v1, const T& v2, T epsilon = T(0.0001))
            {
                return (fabs(v2 - v1) < epsilon);
            }

        template <class T>
            static bool InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon = T(0.0001))
            {
                return (v >= vinf - epsilon && v <= vsup + epsilon);
            }

        template <class T>
            static void Clamp(T& value, const T& vinf, const T& vsup)
            {
                if(value < vinf)
                    value = vinf;
                else if(value > vsup)
                    value = vsup;
            }

        template <class T>
            static T RandomInRange(const T& min, const T& max)
            {
                //return min + (T)((float)rand() * (float)(max - min + 1) / (float)RAND_MAX);
                return min + (T)((float)rand() * (float)(max - min) / (float)RAND_MAX);
            }

        template <class T>
            std::string ToString(const T& v)
            {
                std::ostringstream ss;
                ss << v;
                return v.str();
            }

        template <class T>
            std::string ToNumber(const std::string& v)
            {
                std::ostringstream ss(v);
                T value;
                ss >> value;
                return value;
            }

        static unsigned int SplitString(const std::string &input, const std::string &delim, std::vector<std::string> &output);
        static std::string Trim(const std::string& str);

        static std::string ToLower(std::string str);
        static std::string ToUpper(std::string str);
};

#endif // TOOL_H__
