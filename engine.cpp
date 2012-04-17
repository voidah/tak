#include "engine.h"
#include "tool.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <sstream>



Engine::Engine() : m_wireframe(false), m_takeScreenshot(false), m_synctest(0, 1.0f, 10, &m_syncValueManager)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::cerr << "Error while initializing glew.. abording (" << glewGetErrorString(err) << ")" << std::endl;
        abort();
    }

    // Informations:
    const GLubyte* string;
    string = glGetString(GL_VENDOR);
    printf("Vendor: %s\n", string);
    string = glGetString(GL_RENDERER);
    printf("Renderer: %s\n", string);
    string = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", string);
    string = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("GLSL Version: %s\n", string);

    GLint texSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
    printf("Maximum texture size: %d\n\n", texSize);


    glClearColor(84.f / 255.f, 118.f / 255.f, 244.f / 255.f, 1.f);
    glEnable( GL_TEXTURE_2D );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective( 45.0f, Width() / Height(), 0.01f, 1000.0f);
    gluPerspective( 90.0f, (float)Width() / Height(), 0.01f, 5000.0f);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glEnable (GL_LINE_SMOOTH);

    // TODO

    // TODO
    //unsigned int type = 2;
    //for(int x = 0; x < CHUNK_SIZE_X; ++x)
    //{
    //    for(int z = 0; z < CHUNK_SIZE_Z; ++z)
    //    {
    //        for(int y = 0; y < 32; ++y)
    //        {
    //            if(x % 2 == 0 && z % 2 == 0)
    //                SetBlock(x, y, z, y % BTYPE_LAST);
    //        }
    //    }
    //}
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    // Light
    GLfloat light0Pos[4]  = {0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f};
    GLfloat light0Amb[4]  = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat light0Diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0Spec[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

    CenterMouse();
    HideCursor();

    CHECK_GL_ERROR();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
    LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
    CHECK_GL_ERROR();

    //LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
    //CHECK_GL_ERROR();
    //std::cout << "Loading and compiling shaders..." << std::endl;
    //if(!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
    //{
        //std::cout << "Failed to load shader" << std::endl;
        //exit(1);
    //}
    //CHECK_GL_ERROR();
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
    if(elapsedTime == 0)
        return;

    m_syncValueManager.Update(elapsedTime);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations initiales
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    DrawHud();
    if(m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if(m_takeScreenshot)
    {
        TakeScreenshot("screenshot");
        m_takeScreenshot = false;
    }
}

void Engine::KeyPressEvent(unsigned char key)
{
    switch(key)
    {
        case 0:	    // ESC
            Stop();
            break;
        case 58:	// F10
            SetFullscreen(!IsFullscreen());
            break;
        case 112:       // P
            m_takeScreenshot = true;
            break;
        default:
            std::cout << "Unhandled key: " << (int)key << std::endl;
    }

}

void Engine::KeyReleaseEvent(unsigned char key)
{
    switch(key)
    {
        case 121:       // Y
            m_wireframe = !m_wireframe;
            if(m_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
}

void Engine::MouseMoveEvent(int x, int y)
{
    int dx = x;
    int dy = y;
    MakeRelativeToCenter(dx, dy);

    CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    switch(button)
    {
        case MOUSE_BUTTON_LEFT:
            break;
        case MOUSE_BUTTON_RIGHT:
            break;
        case MOUSE_BUTTON_MIDDLE:
            break;
        default:
            break;
    }
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
}

void Engine::WindowsFocusEvent(bool hasFocus)
{
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
    texture.Load(filename);
    if(!texture.IsValid())
    {
        std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
        if(stopOnError)
            Stop();

        return false;
    }

    return true;
}


void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t)
{
    glLoadIdentity();
    glTranslatef(x, y, 0);
    for(unsigned int i=0; i<t.length(); ++i)
    {
        float left = (float)((t[i] - 32) % 16) / 16.0f;
        float top = (float)((t[i] - 32) / 16) / 16.0f;

        top += 0.5f;

        glBegin(GL_QUADS);
        glTexCoord2f(left, 1.0f - top - 0.0625f);
        glVertex2f(0, 0);
        glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
        glVertex2f(12, 0);
        glTexCoord2f(left + 0.0625f, 1.0f - top);
        glVertex2f(12, 12);
        glTexCoord2f(left, 1.0f - top);
        glVertex2f(0, 12);
        glEnd();

        glTranslatef(8, 0, 0);
    }
}


void Engine::DrawHud()
{
    // Setter le blend function, tout ce qui sera noir sera transparent
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, Width(), 0, Height(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Bind de la texture pour le font
    m_textureFont.Bind();

    std::ostringstream ss;

    ss << PROJECT_NAME << " - v" << MAJOR_VERSION << "." << MINOR_VERSION;
    ss << " (" << __DATE__ << " - " << __TIME__ << ")";
    PrintText(10, Height() - 20, ss.str());


    static int lowFps = 99999;
    static int highFps = 0;
    int curFps = GetFps();
    if(curFps < lowFps && curFps > 10)
        lowFps = curFps;
    highFps = std::max(highFps, curFps);
    ss.str("");
    ss << "Fps: " << GetFps() << " (min=" << lowFps << ", max=" << highFps << ")";
    PrintText(10, Height() - 35, ss.str());

    //ss.str("");
    //ss << "View distance: " << VIEW_DISTANCE;
    //PrintText(10, 55, ss.str());



    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);



    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}

