#ifndef SYNCVALUEMANAGER_H__
#define SYNCVALUEMANAGER_H__

#include "syncvalue.h"
#include <set>

class SyncValueManager
{
    public:
        bool Add(SyncValue* sv);
        bool Remove(SyncValue* sv);
        void Update(float elapsedTime);

    private:
        typedef std::set<SyncValue*> SyncValues;
        SyncValues m_syncValues;
};

#endif // SYNCVALUEMANAGER_H__
