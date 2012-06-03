#include "sceneparams.h"
#include <cassert>

SceneParams::SceneParams() : m_width(0), m_height(0)
{
}

void SceneParams::SetWidth(int v)
{
    m_width = v;
}

int SceneParams::GetWidth() const
{
    assert(m_width != 0);
    return m_width;
}

void SceneParams::SetHeight(int v)
{
    m_height = v;
}

int SceneParams::GetHeight() const
{
    assert(m_height != 0);
    return m_height;
}
