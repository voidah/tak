#ifndef PARTICLESYSTEM_H__
#define PARTICLESYSTEM_H__

#include "particle.h"
#include "uniqueidgenerator.h"
#include "texture.h"
#include "vector3.h"
#include <list>

// Every particle system type should inherit that class
class ParticleSystem
{
    public:
        typedef unsigned int IdType;

    public:
        ParticleSystem(const Vector3f& position, const Texture& texture);
        virtual ~ParticleSystem();

        int GetId() const; 
        const Vector3f& GetPosition() const; 

        void BindTexture() const;

        bool Update(float elapsedTime);
        void Render();

    protected:
        typedef std::list<Particle> ParticleList;

        virtual bool Update(float elapsedTime, ParticleList& pl) = 0;

    private:
        Vector3f m_position;
        const Texture& m_texture;

        static UniqueIdGenerator<IdType> m_idGenerator;
        IdType m_id;

        ParticleList m_particles;
};

#endif // PARTICLESYSTEM_H__
