#ifndef TAK_RESOURCEMANAGER_H__
#define TAK_RESOURCEMANAGER_H__

#include "nocopy.h"
#include "tool.h"
#include <iostream>
#include <string>
#include <map>
#include <cassert>

template <class T>
class ResourceManager
{
    public:
        class Options
        {
            public:
                typedef std::map<std::string, std::string> OptionMap;
                typedef OptionMap::const_iterator ConstIterator;

            public:
                void Set(const std::string& key, const std::string& value)
                {
                    m_options.insert(std::make_pair(key, value));
                }

                bool IsPresent(const std::string& key)
                {
                    return (m_options.find(key) != m_options.end());
                }

                template <class U>
                    U GetNumber(const std::string& key, const U& defaultValue = U(0)) const
                    {
                        OptionMap::const_iterator it = m_options.find(key);
                        if(it == m_options.end())
                            return defaultValue;

                        return Tool::ToNumber<U>(it->second);
                    }

                const std::string& GetString(const std::string& key, const std::string& defaultValue = "") const
                {
                    typename OptionMap::const_iterator it = m_options.find(key);
                    if(it == m_options.end())
                        return defaultValue;

                    return it->second;
                }

                bool GetBoolean(const std::string& key, bool defaultValue = false) const
                {
                    typename OptionMap::const_iterator it = m_options.find(key);
                    if(it == m_options.end())
                        return defaultValue;

                    std::string value = Tool::ToLower(it->second);

                    if(value == "0" || value == "false" || value == "no")
                        return false;

                    // Anything else returns true, even an empty value
                    return true;
                }

                ConstIterator Begin() const
                {
                    return m_options.begin();
                }

                ConstIterator End() const
                {
                    return m_options.end();
                }

                void Output() const
                {
                    for(typename OptionMap::const_iterator it = m_options.begin(); it != m_options.end(); ++it)
                        std::cout << "'" << it->first << "' = '" << it->second << "'" << std::endl;
                }

            private:
                OptionMap m_options;
        };

    public:
        ~ResourceManager();
        static T* Get(const std::string& key);

        static void SetBasePath(const std::string& basePath);

    private:
        typedef std::map<std::string, T*> Resources;

    private:
        static Resources m_resources;
        static std::string m_basePath;

        NOCOPY(ResourceManager);
};

template <class T>
typename ResourceManager<T>::Resources ResourceManager<T>::m_resources;

template <class T>
typename std::string ResourceManager<T>::m_basePath;

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
    std::vector<std::string> tokens;
    Tool::SplitString(key, "|", tokens);

    std::string name = Tool::Trim(tokens[0]);
    assert(name.length() > 0);

    bool relativeToBasePath = false;
    if(name[0] == '!')
    {
        name = name.substr(1);
        assert(name.length() > 0);
        relativeToBasePath = true;
    }

    // Fill options list
    Options options;
    for(int i = 1; i < tokens.size(); ++i)
    {
        std::string::size_type pos = tokens[i].find('=');
        if(pos != std::string::npos)
            options.Set(Tool::Trim(tokens[i].substr(0, pos)), Tool::Trim(tokens[i].substr(pos + 1)));
        else
            options.Set(Tool::Trim(tokens[i]), "");
    }

    // Build stdKey, which will be used to find back the resource
    // The key should be rebuilded to trim spaces, etc between options
    // so that if the same file resource, with the same options is requested,
    // we can find it in the map. Added spaces between options should not result
    // in a different resource
    std::string stdKey(Tool::Trim(tokens[0]));
    for(typename Options::ConstIterator it = options.Begin(); it != options.End(); ++it)
    {
        stdKey += "|" + it->first + (it->second != "" ? "=" + it->second : "");
    }

    typename Resources::iterator it = m_resources.find(stdKey);
    if(it != m_resources.end())
        return it->second;

    //options.Output();

    std::cout << "Loading resource '" << stdKey << "'" << std::endl;
    T* resource = new T();
    if(!resource->Load((relativeToBasePath ? m_basePath : "") + name, options))
    {
        delete resource;
        return 0;
    }

    m_resources.insert(std::make_pair(stdKey, resource));
    return resource;
}

    template <class T>
void ResourceManager<T>::SetBasePath(const std::string& basePath)
{
    m_basePath = basePath;

    // Replace all backslashes by slashes (even on windows, to have uniform path)
    for(int i = 0; i < m_basePath.length(); ++i)
        if(m_basePath[i] == '\\')
            m_basePath = '/';

    // Terminate path by a slash if it's not already the case
    char ending = m_basePath[m_basePath.length() - 1];
    if(ending != '/' && ending != '\\')
        m_basePath += '/';
}


#endif // TAK_RESOURCEMANAGER_H__
