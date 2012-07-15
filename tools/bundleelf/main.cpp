#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstdlib>
#include <set>

std::string ReadCommandOutput(const std::string& command)
{
    std::string result;

    FILE* fp = popen(command.c_str(), "r");
    if(!fp)
        return result;

    char buffer[4096];
    int len;
    while((len = fread(buffer, 1, sizeof(buffer) - 1, fp)) != 0)
    {
        buffer[len] = '\0';
        result += buffer;
    }

    pclose(fp);
    return result;
}

std::string Trim(const std::string& str)
{
    std::string result;
    std::string::size_type pos1 = 0, pos2 = str.length();

    if(pos2 == 0)
        return str;

    while(pos1 < str.size() && (str[pos1] == ' ' || str[pos1] == '\t' || str[pos1] == '\r' || str[pos1] == '\n'))
        ++pos1;

    pos2--;
    while(pos2 > 0 && (str[pos2] == ' ' || str[pos2] == '\t' || str[pos2] == '\r' || str[pos2] == '\n'))
        --pos2;

    return str.substr(pos1, pos2 - pos1 + 1);
}

typedef std::set<std::string> Dependencies;

void GetDependencyList(const std::string& elf, Dependencies& dep)
{
    std::string output = ReadCommandOutput("ldd " + elf);

    std::istringstream ss(output);
    std::string line;
    while(std::getline(ss, line))
    {
        std::string::size_type pos = line.find("=>");
        if(pos == std::string::npos)
            continue;

        pos += 2;
        std::string::size_type pos2 = line.find("(", pos);
        if(pos2 != std::string::npos)
        {
            std::string lib = Trim(line.substr(pos, pos2 - pos));
            if(dep.find(lib) == dep.end())
            {
                std::cout << "New dependency: " << elf << " depends on " << lib << std::endl;
                dep.insert(lib);
                GetDependencyList(lib, dep);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        std::cout << "Usage: bundleelf <executable>" << std::endl;
        return 1;
    }

    std::string exe = argv[1];

    Dependencies dep;
    GetDependencyList(exe, dep);

    for(Dependencies::const_iterator it = dep.begin(); it != dep.end(); ++it)
    {
        system(("cp \"" + *it + "\" .").c_str());
    }
}
