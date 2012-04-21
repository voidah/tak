#include "particlemanager.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::AddParticleSystem(ParticleSystem* ps)
{
    m_systems[ps->GetId()] = ps;
}

void ParticleManager::RemoveParticleSystem(const ParticleSystem::IdType& id, bool autoDelete)
{
    ParticleSystems::iterator it = m_systems.find(id);
    if(it != m_systems.end())
    {
        if(autoDelete)
            delete it->second;
        m_systems.erase(it);
    }
}

void ParticleManager::Update(float elapsedTime)
{
    std::vector<ParticleSystem::IdType> toRemove;
    for(ParticleSystems::iterator it = m_systems.begin(); it != m_systems.end(); ++it)
    {
        if(!it->second->Update(elapsedTime))
            toRemove.push_back(it->first);
    }

    for(int i = 0; i < toRemove.size(); ++i)
        RemoveParticleSystem(toRemove[i], true);
}

void ParticleManager::Render(const Vector3f& playerPosition)
{
    // TODO optimiser pour reduire le nombre de bind de texture (renderer tout ceux qui ont la meme texture
    // en groupe)
    for(ParticleSystems::iterator it = m_systems.begin(); it != m_systems.end(); ++it)
    {
        glPushMatrix();

        const Vector3f& position = it->second->GetPosition();
        /*

        Vector3f objToCamProj = playerPosition - position;
        objToCamProj.y = 0;
        Vector3f lookAt(0, 0, 1);
        objToCamProj.Normalize();
        Vector3f upAux = lookAt.Cross(objToCamProj);
        float angleCosine = lookAt.Dot(objToCamProj);
        //if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
        {
            glTranslatef(position.x, position.y, position.z);
            glRotatef(acos(angleCosine)*180/3.14,upAux.x, upAux.y, upAux.z);
        }

        // Faire le bloc de code suivant seulement si on veut un billboard spherical
        // si on veut juste un cylindrique, ne pas le faire
        if(false)
        {
            Vector3f objToCam = playerPosition - position;
            objToCam.Normalize();

            angleCosine = objToCamProj.Dot(objToCam);
            if(objToCam.y < 0)
                glRotatef(acos(angleCosine)*180/3.14,1,0,0);	
            else
                glRotatef(acos(angleCosine)*180/3.14,-1,0,0);
        }
        */

        glTranslatef(position.x, position.y, position.z);
        it->second->BindTexture();
        it->second->Render();

        glPopMatrix();

    }
}
