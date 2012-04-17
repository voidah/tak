#include "syncvalue.h"
#include "syncvaluemanager.h"

SyncValue::SyncValue(SyncValueManager* svm) : m_syncValueManager(svm)
{
    if(m_syncValueManager)
        m_syncValueManager->Add(this);
}

SyncValue::~SyncValue()
{
    if(m_syncValueManager)
        m_syncValueManager->Remove(this);
}


