#include "openglcontext.h"
#include "define.h"
#include <fstream>

OpenglContext::OpenglContext() : m_maxFps(999999), m_fullscreen(false), m_antialiasingFactor(0), m_title(""), m_lastFrameTime(0)
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

    sf::Clock clock;

    while (m_app.isOpen())
    {
        sf::Event Event;
        while (m_app.pollEvent(Event))
        {
            switch(Event.type)
            {
            case sf::Event::Closed:
                m_app.close();
                break;
            case sf::Event::Resized:
                glViewport(0, 0, Event.size.width, Event.size.height);
                break;
            case sf::Event::KeyPressed:
                KeyPressEvent(Event.key.code);
                break;
            case sf::Event::KeyReleased:
                KeyReleaseEvent(Event.key.code);
                break;
            case sf::Event::MouseMoved:
                MouseMoveEvent(Event.mouseMove.x, Event.mouseMove.y);
                break;
            case sf::Event::MouseButtonPressed:
                MousePressEvent(ConvertMouseButton(Event.mouseButton.button), Event.mouseButton.x, Event.mouseButton.y);
                break;
            case sf::Event::MouseButtonReleased:
                MouseReleaseEvent(ConvertMouseButton(Event.mouseButton.button), Event.mouseButton.x, Event.mouseButton.y);
                break;
            case sf::Event::MouseWheelMoved:
                if(Event.mouseWheel.delta > 0)
                    MousePressEvent(MOUSE_BUTTON_WHEEL_UP, Event.mouseButton.x, Event.mouseButton.y);
                else
                    MousePressEvent(MOUSE_BUTTON_WHEEL_DOWN, Event.mouseButton.x, Event.mouseButton.y);
                break;
            case sf::Event::GainedFocus:
                WindowsFocusEvent(true);
                break;
            case sf::Event::LostFocus:
                WindowsFocusEvent(false);
                break;
            }
        }

        m_app.setActive();
        m_lastFrameTime = clock.getElapsedTime().asSeconds();
        Render(m_lastFrameTime);
        clock.restart();
        m_app.display();
    }

    UnloadResource();
    DeInit();

    return true;
}

bool OpenglContext::Stop()
{
    m_app.close();
    return true;
}

void OpenglContext::CenterMouse()
{
    sf::Mouse::setPosition(sf::Vector2i(Width() / 2, Height() / 2), m_app);
}

int OpenglContext::Width() const
{
    return m_app.getSize().x;
}

int OpenglContext::Height() const
{
    return m_app.getSize().y;
}

void OpenglContext::SetMaxFps(int maxFps)
{
    m_maxFps = maxFps;
    m_app.setFramerateLimit(maxFps);
}

int OpenglContext::GetMaxFps() const
{
    return m_maxFps;
}

int OpenglContext::GetFps() const
{
    return (int)(1.f / m_lastFrameTime);
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
    m_app.setMouseCursorVisible(true);
}

void OpenglContext::HideCursor()
{
    m_app.setMouseCursorVisible(false);
}

void OpenglContext::ShowCrossCursor() const
{
    // TODO
}

void OpenglContext::InitWindow(int width, int height)
{
    m_app.create(sf::VideoMode(width, height, 32), m_title.c_str(), m_fullscreen ? sf::Style::Fullscreen : (sf::Style::Resize|sf::Style::Close), sf::ContextSettings(32, 8, m_antialiasingFactor));
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

