#ifndef TAK_RESOURCEMANAGER_H__
#define TAK_RESOURCEMANAGER_H__

#include "nocopy.h"
#include <string>
#include <map>

template <class T>
class ResourceManager
{
    public:
        ~ResourceManager();
        static T* Get(const std::string& key);

    private:
        typedef std::map<std::string, T*> Resources;

    private:
        static Resources m_resources;

        NOCOPY(ResourceManager);
};

template <class T>
typename ResourceManager<T>::Resources ResourceManager<T>::m_resources;

    template <class T>
ResourceManager<T>::~ResourceManager()
{
    for(typename Resources::iterator it = m_resources.begin(); it != m_resources.end(); ++it)
    {
        it->second->Release();
        delete it->second;
    }
}

    template <class T>
T* ResourceManager<T>::Get(const std::string& key)
{
    typename Resources::iterator it = m_resources.find(key);
    if(it != m_resources.end())
        return it->second;

    T* resource = new T();
    if(!resource->Load(key))
    {
        delete resource;
        return 0;
    }

    m_resources.insert(std::make_pair(key, resource));
    return resource;
}


#endif // TAK_RESOURCEMANAGER_H__
