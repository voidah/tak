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
        typedef uint32 SequentialKeyType;

        static const SequentialKeyType INVALID_KEY = (SequentialKeyType)-1;

    public:
        Resource();
        virtual ~Resource();

        // Those methods should never be called directly, resource
        // creation and deletion must all be done by the ResourceManager
        virtual bool Load(const std::string& key, const OptionList& options) = 0;
        virtual bool Release() = 0;

        static T* Get(const std::string& key);

        SequentialKeyType GetSequentialKey() const;

        // Resources are not copyable directly
        NOCOPY(Resource);

    private:
        // This number is a (almost) sequential number starting at 0, and is set
        // by the ResourceManager when it first load the resource
        // It is used in the sort key when rendering the scene graph
        SequentialKeyType m_sequentialKey;

        friend class ResourceManager<T>;
};

    template <class T>
Resource<T>::Resource() : m_sequentialKey(INVALID_KEY)
{
}

    template <class T>
Resource<T>::~Resource()
{
}

    template <class T>
T* Resource<T>::Get(const std::string& key)
{
    return ResourceManager<T>::Get(key);
}

template <class T>
typename Resource<T>::SequentialKeyType Resource<T>::GetSequentialKey() const
{
    return m_sequentialKey;
}

#endif // TAK_RESOURCE_H__
