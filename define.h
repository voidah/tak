#ifndef TAK__DEFINE_H__
#define TAK__DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "vector3.h"
#include <cmath>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

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

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define DEGTORAD(x) ((x * M_PI) / 180.0)
#define RADTODEG(x) ((180.0 * x) / M_PI)

#define EPSILON 0.001f

#endif // DEFINE_H__
