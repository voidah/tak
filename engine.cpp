#include "engine.h"
#include "tool.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include "game.h"



Engine::Engine(Game* game) : m_game(game), m_wireframe(false), m_takeScreenshot(false)
{
    game->m_syncValueManager = &m_syncValueManager;
    game->m_particleManager = &m_particleManager;
    game->m_engine = this;
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


    glClearColor(m_game->Config.ClearColor.x, m_game->Config.ClearColor.y, m_game->Config.ClearColor.z, 1.f);
    glEnable( GL_TEXTURE_2D );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective( 45.0f, Width() / Height(), 0.01f, 1000.0f);
    gluPerspective( 90.0f, (float)Width() / Height(), 0.01f, 5000.0f);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glEnable (GL_LINE_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    // Light
    GLfloat light0Pos[4]  = {0.0f, 50.f, 0.0f, 1.0f};
    GLfloat light0Amb[4]  = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat light0Diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0Spec[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

    //CenterMouse();
    //HideCursor();

    CHECK_GL_ERROR();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
    LoadTexture(m_textureFont, "../../tak/resource/font.bmp");
    CHECK_GL_ERROR();

    m_game->LoadResource();
}

void Engine::UnloadResource()
{
    m_game->UnloadResource();
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

    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    m_game->Render3d(elapsedTime);
    glDisable(GL_BLEND);
    glPopMatrix();

    if(m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Render2d(elapsedTime);
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
        case 36:	    // ESC
            Stop();
            break;
        case 94:	// F10
            SetFullscreen(!IsFullscreen());
            break;
        case 15:       // P
            m_takeScreenshot = true;
            break;
        default:
            std::cout << "Unhandled key: " << (int)key << std::endl;
    }

    m_game->KeyPressEvent(key);
}

void Engine::KeyReleaseEvent(unsigned char key)
{
    switch(key)
    {
        case 24:       // Y
            m_wireframe = !m_wireframe;
            if(m_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }

    m_game->KeyReleaseEvent(key);
}

void Engine::MouseMoveEvent(int x, int y)
{
    m_game->MouseMoveEvent(x, y);
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    m_game->MousePressEvent(button, x, y);
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
    m_game->MouseReleaseEvent(button, x, y);
}

void Engine::WindowFocusEvent(bool hasFocus)
{
    m_game->WindowFocusEvent(hasFocus);
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


void Engine::PrintText(int x, int y, const std::string& t)
{
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, 0);
    for(unsigned int i=0; i<t.length(); ++i)
    {
        float left = (float)((t[i] - 32) % 16) / 16.0f;
        float top = (float)((t[i] - 32) / 16) / 16.0f;

        top += 0.5f;

        glBegin(GL_QUADS);
        glTexCoord2f(left, 1.0f - top - 0.0625f);
        glVertex2f(0, 12);
        glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
        glVertex2f(12, 12);
        glTexCoord2f(left + 0.0625f, 1.0f - top);
        glVertex2f(12, 0);
        glTexCoord2f(left, 1.0f - top);
        glVertex2f(0, 0);
        glEnd();

        glTranslatef(8, 0, 0);
    }
    glPopMatrix();
}


void Engine::Render2d(float elapsedTime)
{
    //=========================================================================
    // Set projection and modelview matrix
    //=========================================================================
    CHECK_GL_ERROR();
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, Width(), Height(), 0, -1000.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    CHECK_GL_ERROR();

    //=========================================================================
    // Call game's Render2d method
    //=========================================================================
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPushMatrix();
    m_game->Render2d(elapsedTime);
    glPopMatrix();
    glDisable(GL_BLEND);
    CHECK_GL_ERROR();

    //=========================================================================
    // Show particles
    //=========================================================================
    glLoadIdentity();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING); // TODO
    //glDisable(GL_CULL_FACE); // TODO
    CHECK_GL_ERROR();
    m_particleManager.Update(elapsedTime);
    CHECK_GL_ERROR();
    //m_particleManager.Render(m_player.Position());
    m_particleManager.Render(Vector3f(0.f, 0, 50.f));
    CHECK_GL_ERROR();
    //glEnable(GL_CULL_FACE); // TODO
    glEnable(GL_LIGHTING); // TODO
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    CHECK_GL_ERROR();


    //=========================================================================
    // Text
    //=========================================================================
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    CHECK_GL_ERROR();

    m_textureFont.Bind();
    CHECK_GL_ERROR();
    std::ostringstream ss;

    int offset = 20;
    if(m_game->Config.ShowVersion)
    {
        ss << m_game->GetName() << " - v" << m_game->GetVersion();
        ss << " (" << __DATE__ << " - " << __TIME__ << ")";
        PrintText(10, offset, ss.str());
        offset += 15;
    }

    static int lowFps = 99999;
    static int highFps = 0;
    if(m_game->Config.ShowFps)
    {
        int curFps = GetFps();
        if(curFps < lowFps && curFps > 10)
            lowFps = curFps;
        highFps = std::max(highFps, curFps);
        ss.str("");
        ss << "Fps: " << GetFps() << " (min=" << lowFps << ", max=" << highFps << ")";
        PrintText(10, offset, ss.str());
    }
    CHECK_GL_ERROR();

    m_game->RenderText(elapsedTime);

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERROR();

    //=========================================================================
    // Restore projection and modelview matrix
    //=========================================================================
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    CHECK_GL_ERROR();
}

