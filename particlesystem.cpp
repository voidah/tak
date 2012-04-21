#include "particlesystem.h"

UniqueIdGenerator<ParticleSystem::IdType> ParticleSystem::m_idGenerator;

ParticleSystem::ParticleSystem(const Vector3f& position, const Texture& texture) : m_position(position), m_texture(texture)
{
    m_id = m_idGenerator.Get();
}

ParticleSystem::~ParticleSystem()
{
}

int ParticleSystem::GetId() const
{
    return m_id;
}

const Vector3f& ParticleSystem::GetPosition() const
{
    return m_position;
}

void ParticleSystem::BindTexture() const
{
    m_texture.Bind();
}

bool ParticleSystem::Update(float elapsedTime)
{
    return Update(elapsedTime, m_particles);
}

void ParticleSystem::Render()
{
    glBegin(GL_QUADS);

    // TODO user vbo instead of direct mode...
    for(ParticleList::const_iterator it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        const Particle& p = *it;

        float x = p.position.x;
        float y = p.position.y;
        float z = p.position.z;
        float hw = p.width / 2.f;
        float hh = p.height / 2.f;

        //glNormal3f(0, 0, 1);            // Normal vector
        glTexCoord2f(0.f, 0.f);
        glVertex3f(x - hw, y + hh, z);
        glTexCoord2f(1.f, 0.f);
        glVertex3f(x + hw, y + hh, z);
        glTexCoord2f(1.f, 1.f);
        glVertex3f(x + hw, y - hh, z);
        glTexCoord2f(0.f, 1.f);
        glVertex3f(x - hw, y - hh, z);
    }

    glEnd();
}
