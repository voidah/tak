#include "syncvaluemanager.h"

bool SyncValueManager::Add(SyncValue* sv)
{
    m_syncValues.insert(sv);
    return true;
}

bool SyncValueManager::Remove(SyncValue* sv)
{
    //SyncValues::iterator it = m_syncValues.find(sv);
    //if(it != m_syncValues.end())
        //m_syncValues.erase(it);
    m_syncValues.erase(sv);
    return true;
}

void SyncValueManager::Update(float elapsedTime)
{
    for(SyncValues::iterator it = m_syncValues.begin(); it != m_syncValues.end(); ++it)
        (*it)->Update(elapsedTime);
}

