#include "engine.h"
#include "tool.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include "game.h"
#include "physicengine.h"



Engine::Engine(Game* game) : m_game(game), m_wireframe(false), m_takeScreenshot(false)
{
    game->m_syncValueManager = &m_syncValueManager;
    game->m_scene = &m_scene;
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

    GPhysicEngine.Init();

    // Informations:
    const GLubyte* string;
    string = glGetString(GL_VENDOR);
    printf("Vendor: %s\n", string);
    string = glGetString(GL_RENDERER);
    printf("Renderer: %s\n", string);
    string = glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", string);

    string = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if(!string)
	{
		std::cout << "Your graphic driver doesn't support shaders, exiting" << std::endl;
		exit(1);
	}
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
    GPhysicEngine.DeInit();
}

void Engine::LoadResource()
{
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

    bool showCursor = m_game->Config.ShowCursor;
    if(IsCursorVisible() != showCursor)
    {
        if(showCursor)
            ShowCursor();
        else
            HideCursor();
    }

    m_syncValueManager.Update(elapsedTime);
    m_scene.Update(elapsedTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations initiales
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    m_game->Render(elapsedTime);

    Shader* defaultShader = m_game->GetDefaultShader();
    assert(defaultShader);
    Texture* defaultTexture = m_game->GetDefaultTexture();
    assert(defaultTexture);
    m_scene.Render(defaultShader, defaultTexture);

    glDisable(GL_BLEND);
    glPopMatrix();

    // Physic graphical debug output:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(m_scene.GetCamera())
        glMultMatrixf(m_scene.GetCamera()->GetMatrix().GetInternalValues());
    GPhysicEngine.DrawDebug();

    // Simulate physic
    GPhysicEngine.Update(elapsedTime);

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
    }

    m_scene.KeyPressEvent(key);
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

    m_scene.KeyReleaseEvent(key);
    m_game->KeyReleaseEvent(key);
}

void Engine::MouseMoveEvent(int x, int y)
{
    if(m_game->Config.CenterMouse)
    {
        MakeRelativeToCenter(x, y);

        if(x == 0 && y == 0)
            return;
        CenterMouse();
    }

    m_scene.MouseMoveEvent(x, y);
    m_game->MouseMoveEvent(x, y);
}

void Engine::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    m_scene.MousePressEvent(button, x, y);
    m_game->MousePressEvent(button, x, y);
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
    m_scene.MouseReleaseEvent(button, x, y);
    m_game->MouseReleaseEvent(button, x, y);
}

void Engine::WindowFocusEvent(bool hasFocus)
{
    m_scene.WindowFocusEvent(hasFocus);
    m_game->WindowFocusEvent(hasFocus);
}

void Engine::WindowResizeEvent(int width, int height)
{
    m_scene.WindowResizeEvent(width, height);
    m_game->WindowResizeEvent(width, height);
}


