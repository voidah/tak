#ifndef TAK_NODES_H__
#define TAK_NODES_H__


#include <list>
#include "define.h"
#include "particle.h"



class ContainerNode : public SceneNode
{
    public:
        ContainerNode() : SceneNode("ContainerNode") {}

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params) { return true; }
        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params) {}
};

class OrthographicProjectionNode: public SceneNode
{
    public:
        OrthographicProjectionNode() : SceneNode("OrthographicProjectionNode") {}

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params)
        {
            // TODO do it only once, and when width or height change
            m_projection.SetOrthographicProjection(0, params.GetWidth(), 0, params.GetHeight(), -100.f, 100.f);
            return true;
        }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
        }

        virtual void UpdateProjectionMatrix(Matrix4f& projection, Camera* camera)
        {
            projection = m_projection;
        }

        virtual void UpdateModelviewMatrix(Matrix4f& modelview, Camera* camera)
        {
            modelview = Matrix4f::IDENTITY;
        }


    private:
        Matrix4f m_projection;
};

class Entity : public SceneNode
{
    public:
        Entity(Model* model) : SceneNode("Entity"), m_mesh(0)
    {
        m_mesh = model->GetMesh();
    }

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params) { return true; }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
            m_mesh->Render();
        }

    private:
        Mesh<VertexData3dNormal>* m_mesh;
};

class ParticleSystemNode : public SceneNode
{
    public:
        enum BILLBOARD_TYPE { BT_CYLINDRICAL, BT_SPHERICAL };

    public:
        ParticleSystemNode(const Vector3f& position, Texture* texture, BILLBOARD_TYPE type = BT_CYLINDRICAL) : SceneNode("ParticleSystemNode"), m_mesh(Mesh<VertexData3d>::MT_QUAD), m_type(type)
    {
        SetFlag(FLAG_NO_DEPTH_TEST);
        SetTexture(texture);
        SetPositionAbsolute(position);
    }

    protected:
        typedef std::list<Particle> ParticleList;

    protected:
        virtual bool Update(float elapsedTime, ParticleList& pl) = 0;

    private:
        virtual bool Update(float elapsedTime, SceneParams& params)
        {
            if(Update(elapsedTime, m_particles))
            {
                // Rebuild mesh...
                VertexData3d* vd = new VertexData3d[m_particles.size() * 4];

                int idx = 0;
                for(ParticleList::const_iterator it = m_particles.begin(); it != m_particles.end(); ++it)
                {
                    const Particle& p = *it;

                    float x = p.position.x;
                    float y = p.position.y;
                    float z = p.position.z;
                    float hw = p.width / 2.f;
                    float hh = p.height / 2.f;


                    float ll = 1.f;

                    vd[idx++] = Mesh<VertexData3d>::VertexData(x - hw, y - hh, z, ll, ll, ll, 0, 0);
                    vd[idx++] = Mesh<VertexData3d>::VertexData(x + hw, y - hh, z, ll, ll, ll, 1, 0);
                    vd[idx++] = Mesh<VertexData3d>::VertexData(x + hw, y + hh, z, ll, ll, ll, 1, 1);
                    vd[idx++] = Mesh<VertexData3d>::VertexData(x - hw, y + hh, z, ll, ll, ll, 0, 1);
                }

                m_mesh.SetMeshData(vd, idx);
                delete [] vd;

                return true;
            }            
        }

        virtual void UpdateModelviewMatrix(Matrix4f& modelview, Camera* camera)
        {
            // Reference: http://www.lighthouse3d.com/opengl/billboarding/index.php?billCyl

            const Vector3f& position = GetPosition(); // TODO should be in world space coordinate...
            const Vector3f& cameraPos = camera->GetWorldPosition();

            Vector3f objToCamProj = cameraPos - position;
            objToCamProj.y = 0;
            Vector3f lookAt(0, 0, cameraPos.z - position.z > 0.f ? 1.f : -1.f);

            objToCamProj.Normalize();
            Vector3f upAux = lookAt.Cross(objToCamProj);
            float angleCosine = lookAt.Dot(objToCamProj);
            //if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
            {
                float angle;
                //angle = 360-RADTODEG(acos(angleCosine));
                angle = -RADTODEG(acos(angleCosine));
                if(cameraPos.z - position.z < 0.f)
                    angle += 90;
                //std::cout << angle << std::endl;
                modelview.ApplyRotation(angle, upAux.x, upAux.y, upAux.z);
            }

            if(m_type == BT_SPHERICAL)
            {
                Vector3f objToCam = cameraPos - position;
                objToCam.Normalize();

                angleCosine = objToCamProj.Dot(objToCam);
                if(objToCam.y < 0)
                    modelview.ApplyRotation(RADTODEG(acos(angleCosine)), 1, 0, 0);	
                else
                    modelview.ApplyRotation(RADTODEG(acos(angleCosine)), -1, 0, 0);
            }
        }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
            m_mesh.Render();
        }

    private:
        ParticleList m_particles;
        Mesh<VertexData3d> m_mesh;
        BILLBOARD_TYPE m_type;
};

class SoundNode : public SceneNode
{
    public:
        SoundNode(const Vector3f& position, Sound* sound, bool playNow = true) : SceneNode("SoundNode"), m_sound(sound->GetInternalBuffer())
    {
        SetPositionAbsolute(position);

        if(playNow)
            PlaySound();
    }

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params)
        {
            static Vector3f lastPosition = GetPosition();

            // Si le son ne doit pas jouer en loop, et qu'il est fini de jouer, faire deleter le scene node...
            sf::Sound::Status status = m_sound.getStatus();
            if(status == sf::Sound::Playing)
            {
                // Update sound position only if needed
                Vector3f currentPosition = GetPosition();
                if(lastPosition != currentPosition)
                {
                    UpdatePosition();
                    lastPosition = currentPosition;
                }
            }
            else if(status == sf::Sound::Stopped)
            {
                return false;
            }

            return true;
        }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
        }

    private:
        void UpdatePosition()
        {
            Vector3f pos = GetPosition(); // TODO should be world coordinate position
            m_sound.setPosition(pos.x, pos.y, pos.z);
        }

        void PlaySound()
        {
            UpdatePosition();
            m_sound.play();
        }

    private:
        sf::Sound m_sound;
};

class Sprite: public SceneNode
{
    public:
        Sprite(float width, float height, Texture* texture) : SceneNode("Sprite"), m_width(width), m_height(height), m_mesh(Mesh<VertexData3d>::MT_QUAD)
    {
        SetFlag(FLAG_HUD);
        SetTexture(texture);

        VertexData3d vd[4];

        float hw = width / 2.f;
        float hh = height / 2.f;
        float ll = 1.f;

        vd[0] = Mesh<VertexData3d>::VertexData(-hw, -hh, 0, ll, ll, ll, 0, 0);
        vd[1] = Mesh<VertexData3d>::VertexData(hw, -hh, 0, ll, ll, ll, 1, 0);
        vd[2] = Mesh<VertexData3d>::VertexData(hw, hh, 0, ll, ll, ll, 1, 1);
        vd[3] = Mesh<VertexData3d>::VertexData(-hw, hh, 0, ll, ll, ll, 0, 1);

        m_mesh.SetMeshData(vd, 4);
    }

        float GetWidth() const
        {
            return m_width;
        }

        float GetHeight() const
        {
            return m_height;
        }

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params)
        {
            return true;
        }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
            m_mesh.Render();
        }

    private:
        float m_width;
        float m_height;
        Mesh<VertexData3d> m_mesh;
};

class Cube : public SceneNode
{
    public:
        Cube(const Vector3f& size = Vector3f(1.f, 1.f, 1.f), float textureRepeat = 1.f, const std::string& name = "Cube") : SceneNode(name), m_mesh(Mesh<VertexData3dNormal>::MT_QUAD), m_size(size)
        {
            VertexData3dNormal* vd = new VertexData3dNormal[24];
            int count = 0;
            AddBlockToMesh(vd, count, textureRepeat);
            m_mesh.SetMeshData(vd, count);
            delete [] vd;
        }

        ~Cube()
        {
            if(IsBoundToRigidBody())
            {
                RigidBody* rb = GetBoundRigidBody();
                GPhysicEngine.RemoveRigidBody(rb);
                delete rb;
            }
        }

        const Vector3f& GetSize() const
        {
            return m_size;
        }

        virtual RigidBody* AddToPhysic(float weight, const Vector3f& position)
        {
            BoxRigidBody* br1 = new BoxRigidBody(weight, position, m_size);
            BindToRigidBody(br1);
            GPhysicEngine.AddRigidBody(br1);

            return br1;
        }

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params)
        {
            return true;
        }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
            m_mesh.Render();
        }

    private:
        Mesh<VertexData3dNormal> m_mesh;
        Vector3f m_size;

        void AddBlockToMesh(VertexData3dNormal* vd, int& count, float textureRepeat)
        {
            float iu = 0;
            float iv = 0;
            float ts = 1.f;

            float ll = 1.f;

            float hx = m_size.x * .5f;
            float hy = m_size.y * .5f;
            float hz = m_size.z * .5f;

            // front
            {
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx, -hy,  hz, 0, 0, 1, ll, ll, ll, 0, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx, -hy,  hz, 0, 0, 1, ll, ll, ll, textureRepeat, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx,  hy,  hz, 0, 0, 1, ll, ll, ll, textureRepeat, textureRepeat);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx,  hy,  hz, 0, 0, 1, ll, ll, ll, 0, textureRepeat);
            }

            // back
            {
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx, -hy, -hz, 0, 0, -1, ll, ll, ll, 0, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx, -hy, -hz, 0, 0, -1, ll, ll, ll, textureRepeat, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx,  hy, -hz, 0, 0, -1, ll, ll, ll, textureRepeat, textureRepeat);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx,  hy, -hz, 0, 0, -1, ll, ll, ll, 0, textureRepeat);
            }

            // left
            {
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx, -hy, -hz, -1, 0, 0, ll, ll, ll, 0, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx, -hy,  hz, -1, 0, 0, ll, ll, ll, textureRepeat, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx,  hy,  hz, -1, 0, 0, ll, ll, ll, textureRepeat, textureRepeat);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx,  hy, -hz, -1, 0, 0, ll, ll, ll, 0, textureRepeat);
            }

            // right
            {
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx, -hy,  hz, 1, 0, 0, ll, ll, ll, 0, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx, -hy, -hz, 1, 0, 0, ll, ll, ll, textureRepeat, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx,  hy, -hz, 1, 0, 0, ll, ll, ll, textureRepeat, textureRepeat);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx,  hy,  hz, 1, 0, 0, ll, ll, ll, 0, textureRepeat);
            }

            // top
            {

                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx,  hy,  hz, 0, 1, 0, ll, ll, ll, textureRepeat, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx,  hy,  hz, 0, 1, 0, ll, ll, ll, 0, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx,  hy, -hz, 0, 1, 0, ll, ll, ll, 0, textureRepeat);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx,  hy, -hz, 0, 1, 0, ll, ll, ll, textureRepeat, textureRepeat);
            }

            // bottom
            {
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx, -hy, -hz, 0, -1, 0, ll, ll, ll, textureRepeat, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx, -hy, -hz, 0, -1, 0, ll, ll, ll, 0, 0);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData( hx, -hy,  hz, 0, -1, 0, ll, ll, ll, 0, textureRepeat);
                vd[count++] = Mesh<VertexData3dNormal>::VertexData(-hx, -hy,  hz, 0, -1, 0, ll, ll, ll, textureRepeat, textureRepeat);
            }
        }
};

class TextNode : public SceneNode
{
    public:
        TextNode(Font* font, float size = 12.f, const std::string& text = "") : SceneNode("TextNode"), m_font(font), m_mesh(Mesh<VertexData3d>::MT_QUAD), m_size(size)
        {
            SetFlag(FLAG_HUD);
            SetText(text);
            SetTexture(&font->GetTexture());
        }

        void SetText(const std::string& text)
        {
            // text might be utf8 encoded, make sure it's ascii
            //std::cout << "CODEPAGE: " << m_font.GetAnsiCodePage() << std::endl;
            m_text = Unicode::FromUtf8ToAnsi(text, m_font->GetAnsiCodePage());
            //std::cout << "SetText: " << m_text << std::endl;
            if(m_text.length() == 0)
                return;

            VertexData3d* vd = new VertexData3d[m_text.length() * 4];
            int count = 0;

            float px = 0;
            for(int i = 0; i < m_text.length(); ++i)
            {
                const Font::CharInfo& ci = m_font->GetCharInfo(m_text[i]);
                //std::cout << "SetText char: " << m_text[i] << ", " << ci.charW << ", " << ci.charH << std::endl;
                //std::cout << ci.texX << ", " << ci.texY << ", " << ci.texW << ", " << ci.texH << std::endl;

                float ll = 1.f;
                // TODO rendre counterclock-wise... pour tout ce que je render... corriger partout
                vd[count++] = Mesh<VertexData3d>::VertexData(px, -ci.charB, 0, ll, ll, ll, ci.texX, ci.texY);
                vd[count++] = Mesh<VertexData3d>::VertexData(px + ci.charW, -ci.charB, 0, ll, ll, ll, ci.texX + ci.texW, ci.texY);
                vd[count++] = Mesh<VertexData3d>::VertexData(px + ci.charW, ci.charH - ci.charB, 0, ll, ll, ll, ci.texX + ci.texW, ci.texY + ci.texH);
                vd[count++] = Mesh<VertexData3d>::VertexData(px, ci.charH - ci.charB, 0, ll, ll, ll, ci.texX, ci.texY + ci.texH);

                px += ci.charW;
            }


            assert(count == m_text.length() * 4);
            m_mesh.SetMeshData(vd, count);
            delete [] vd;
        }

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params) { return true; }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
            if(m_text.length() == 0)
                return;

            m_mesh.Render();
        }

    private:
        Mesh<VertexData3d> m_mesh;
        const Font* m_font;
        float m_size;
        std::string m_text;
};

class SpriteRotationNode : public SceneNode
{
    public:
        SpriteRotationNode(float posX, float posY, float posZ, float width, float height) : SceneNode("SpriteRotationNode"), m_mesh(Mesh<VertexData3d>::MT_QUAD)
    {
        SetPositionAbsolute(posX, posY, posZ);

        VertexData3d vd[4];

        float hw = width / 2.f;
        float hh = height / 2.f;
        float ll = 1.f;

        vd[0] = Mesh<VertexData3d>::VertexData(-hw, -hh, 0, ll, ll, ll, 0, 0);
        vd[1] = Mesh<VertexData3d>::VertexData(hw, -hh, 0, ll, ll, ll, 1, 0);
        vd[2] = Mesh<VertexData3d>::VertexData(hw, hh, 0, ll, ll, ll, 1, 1);
        vd[3] = Mesh<VertexData3d>::VertexData(-hw, hh, 0, ll, ll, ll, 0, 1);
        //vd[0] = Mesh<VertexData3d>::VertexData(posX - hw, posY - hh, 0, ll, ll, ll, 0, 0);
        //vd[1] = Mesh<VertexData3d>::VertexData(posX + hw, posY - hh, 0, ll, ll, ll, 1, 0);
        //vd[2] = Mesh<VertexData3d>::VertexData(posX + hw, posY + hh, 0, ll, ll, ll, 1, 1);
        //vd[3] = Mesh<VertexData3d>::VertexData(posX - hw, posY + hh, 0, ll, ll, ll, 0, 1);

        m_mesh.SetMeshData(vd, 4);
    }

    protected:
        virtual bool Update(float elapsedTime, SceneParams& params)
        {
            float rotY = GetRotY() + elapsedTime * 200.f;
            if(rotY >= 90.f)
                rotY = -90.f;
            SetRotationAbsolute(GetRotX(), rotY, GetRotZ());

            //SetRotationRelative(0, 0, elapsedTime * 5.f);
            return true;
        }

        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params)
        {
            m_mesh.Render();
        }

    private:
        Mesh<VertexData3d> m_mesh;
};



#endif // TAK_NODES_H__
