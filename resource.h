#ifndef TAK_RESOURCE_H__
#define TAK_RESOURCE_H__

#include "nocopy.h"
#include <string>
#include "resourcemanager.h"

template <class T>
class Resource
{
    public:
        typedef typename ResourceManager<T>::Options OptionList;

    public:
        Resource() {}
        virtual ~Resource() {}

        // Those methods should never be called directly, resource
        // creation and deletion must all be done by the ResourceManager
        virtual bool Load(const std::string& key, const OptionList& options) = 0;
        virtual bool Release() = 0;

        static T* Get(const std::string& key);

        // Resources are not copyable directly
        NOCOPY(Resource);
};

    template <class T>
T* Resource<T>::Get(const std::string& key)
{
    return ResourceManager<T>::Get(key);
}

#endif // TAK_RESOURCE_H__
