#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "vector3.h"
#include <cmath>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

#define PROJECT_NAME "noname"
#define MAJOR_VERSION 0
#define MINOR_VERSION 1

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;


#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define TEXTURE_PATH        "../tak/media/textures/minecraft4kids/"
#define SHADER_PATH        "../tak/media/shaders/"
#define WORLD_PATH        "../tak/media/worlds/"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define DEGTORAD(x) ((x * M_PI) / 180.0)
#define RADTODEG(x) ((180.0 * x) / M_PI)

#define EPSILON 0.001f

#endif // DEFINE_H__
