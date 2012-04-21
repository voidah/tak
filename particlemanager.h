#ifndef PARTICLEMANAGER_H__
#define PARTICLEMANAGER_H__

#include "particlesystem.h"
#include "vector3.h"
#include <map>

class ParticleManager
{
    public:
        ParticleManager();
        ~ParticleManager();

        void AddParticleSystem(ParticleSystem* ps);
        void RemoveParticleSystem(const ParticleSystem::IdType& id, bool autoDelete = true);

        void Update(float elapsedTime);
        void Render(const Vector3f& playerPosition);

    private:
        typedef std::map<ParticleSystem::IdType, ParticleSystem*> ParticleSystems;
        ParticleSystems m_systems;
};

#endif // PARTICLEMANAGER_H__
