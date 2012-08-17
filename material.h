#ifndef TAK_MATERIAL_H__
#define TAK_MATERIAL_H__

#include "define.h"

class Texture;
class Shader;

class Material
{
    public:
        Texture* texture;
        Shader* shader;

        Material();
};

#endif // TAK_MATERIAL_H__
