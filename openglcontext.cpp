#include "openglcontext.h"
#include "define.h"
#include <fstream>

OpenglContext::OpenglContext() : m_maxFps(999999), m_fullscreen(false), m_antialiasingFactor(0), m_title("")
{
}

OpenglContext::~OpenglContext()
{
}

bool OpenglContext::Start(const std::string& title, int width, int height, bool fullscreen, int antialiasingFactor)
{
    m_title = title;
    m_fullscreen = fullscreen;
    m_antialiasingFactor = antialiasingFactor;
    InitWindow(width, height);

    Init();
    LoadResource();

    while (m_app.IsOpened())
    {
        sf::Event Event;
        while (m_app.GetEvent(Event))
        {
            switch(Event.Type)
            {
            case sf::Event::Closed:
                m_app.Close();
                break;
            case sf::Event::Resized:
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
                break;
            case sf::Event::KeyPressed:
                KeyPressEvent(Event.Key.Code);
                break;
            case sf::Event::KeyReleased:
                KeyReleaseEvent(Event.Key.Code);
                break;
            case sf::Event::MouseMoved:
                MouseMoveEvent(Event.MouseMove.X, Event.MouseMove.Y);
                break;
            case sf::Event::MouseButtonPressed:
                MousePressEvent(ConvertMouseButton(Event.MouseButton.Button), Event.MouseButton.X, Event.MouseButton.Y);
                break;
            case sf::Event::MouseButtonReleased:
                MouseReleaseEvent(ConvertMouseButton(Event.MouseButton.Button), Event.MouseButton.X, Event.MouseButton.Y);
                break;
            case sf::Event::MouseWheelMoved:
                if(Event.MouseWheel.Delta > 0)
                    MousePressEvent(MOUSE_BUTTON_WHEEL_UP, Event.MouseButton.X, Event.MouseButton.Y);
                else
                    MousePressEvent(MOUSE_BUTTON_WHEEL_DOWN, Event.MouseButton.X, Event.MouseButton.Y);
                break;
            case sf::Event::GainedFocus:
                WindowsFocusEvent(true);
                break;
            case sf::Event::LostFocus:
                WindowsFocusEvent(false);
                break;
            }
        }

        m_app.SetActive();
        Render(m_app.GetFrameTime());
        m_app.Display();
    }

    UnloadResource();
    DeInit();

    return true;
}

bool OpenglContext::Stop()
{
    m_app.Close();
    return true;
}

void OpenglContext::CenterMouse()
{
    m_app.SetCursorPosition(Width() / 2, Height() / 2);
}

int OpenglContext::Width() const
{
    return m_app.GetWidth();
}

int OpenglContext::Height() const
{
    return m_app.GetHeight();
}

void OpenglContext::SetMaxFps(int maxFps)
{
    m_maxFps = maxFps;
    m_app.SetFramerateLimit(maxFps);
}

int OpenglContext::GetMaxFps() const
{
    return m_maxFps;
}

int OpenglContext::GetFps() const
{
    return (int)(1.f / m_app.GetFrameTime());
}

void OpenglContext::SetFullscreen(bool fullscreen)
{
    if(m_fullscreen == fullscreen)
        return;

    m_fullscreen = !m_fullscreen;

    DeInit();
    InitWindow(Width(), Height());
    Init();
}

bool OpenglContext::IsFullscreen() const
{
    return m_fullscreen;
}

bool OpenglContext::TakeScreenshot(const std::string& fname) const
{
    char* data = new char[Width() * Height() * 3];

    glReadPixels(0, 0, Width(), Height(), GL_BGR, GL_UNSIGNED_BYTE, data);

    std::ofstream f((fname + ".tga").c_str());
    if(!f.is_open())
        return false;

    f.put(0);  // id = 0 bytes
    f.put(0);  // colormap
    f.put(2);  // type = true color
    f.put(0);  // colormap spec byte 1
    f.put(0);  // colormap spec byte 2
    f.put(0);  // colormap spec byte 3
    f.put(0);  // colormap spec byte 4
    f.put(0);  // colormap spec byte 5
    f.put(0);  // x origin
    f.put(0);  // x origin
    f.put(0);  // y origin
    f.put(0);  // y origin
    f.put(Width() & 0xff);  // width
    f.put((Width() & 0xff00) >> 8);  // Height
    f.put(Height() & 0xff);  // width
    f.put((Height() & 0xff00) >> 8);  // Height
    f.put(24);  // bit depth
    f.put(0);   // image description

    f.write(data, Width() * Height() * 3);
    f.close();


    //sf::Image Screen;
    //sf::Image Screen = m_app.Capture();
    //Screen.SaveToFile(fname);
    delete [] data;
    return true;
}

void OpenglContext::MakeRelativeToCenter(int& x, int& y) const
{
    x = x - (Width() / 2);
    y = y - (Height() / 2);
}

void OpenglContext::ShowCursor()
{
    m_app.ShowMouseCursor(true);
}

void OpenglContext::HideCursor()
{
    m_app.ShowMouseCursor(false);
}

void OpenglContext::ShowCrossCursor() const
{
    // TODO
}

void OpenglContext::InitWindow(int width, int height)
{
    m_app.Create(sf::VideoMode(width, height, 32), m_title.c_str(), m_fullscreen ? sf::Style::Fullscreen : (sf::Style::Resize|sf::Style::Close), sf::WindowSettings(32, 8, m_antialiasingFactor));
}

OpenglContext::MOUSE_BUTTON OpenglContext::ConvertMouseButton(sf::Mouse::Button button) const
{
    switch(button)
    {
    case sf::Mouse::Left:
        return MOUSE_BUTTON_LEFT;
    case sf::Mouse::Middle:
        return MOUSE_BUTTON_MIDDLE;
    case sf::Mouse::Right:
        return MOUSE_BUTTON_RIGHT;
    default:
        return MOUSE_BUTTON_NONE;
    }
}

