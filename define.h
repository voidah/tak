#ifndef TAK__DEFINE_H__
#define TAK__DEFINE_H__

#include "config.h"

// OS detection
#if(defined(_WIN32))
#define OS_WINDOWS
#elif(defined(__gnu_linux__))
#define OS_LINUX
#else
#error "Could not determine operating system"
#endif

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "vector3.h"
#include <cmath>
#include <climits>

#ifdef OS_WINDOWS
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
#define M_PI           3.1415926535897932384626433832795
#endif

#define PI_OVER_180	 0.017453292519943295769236907684886
#define PI_OVER_360	 0.0087266462599716478846184538424431

#define DEGTORAD(x) ((x * M_PI) / 180.0)
#define RADTODEG(x) ((180.0 * x) / M_PI)

#define EPSILON 0.001f

enum MOUSE_BUTTON {
    MOUSE_BUTTON_NONE       = 0x00,
    MOUSE_BUTTON_LEFT       = 0x01,
    MOUSE_BUTTON_MIDDLE     = 0x02,
    MOUSE_BUTTON_RIGHT      = 0x04,
    MOUSE_BUTTON_WHEEL_UP   = 0x08,
    MOUSE_BUTTON_WHEEL_DOWN = 0x10
};

#endif // DEFINE_H__
