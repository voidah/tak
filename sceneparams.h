#ifndef SCENEPARAMS_H__
#define SCENEPARAMS_H__

class Texture;
class Shader;
class Camera;

class SceneParams
{
    public:
        SceneParams();

        void SetWidth(int v);
        int GetWidth() const;
        void SetHeight(int v);
        int GetHeight() const;

        void SetDefaultTexture(Texture* texture);
        Texture* GetDefaultTexture() const;
        void SetCurrentTexture(Texture* texture);
        Texture* GetCurrentTexture() const;

        void SetDefaultShader(Shader* shader);
        Shader* GetDefaultShader() const;
        void SetCurrentShader(Shader* shader);
        Shader* GetCurrentShader() const;

        void SetCamera(Camera* camera);
        Camera* GetCamera() const;

    private:
        int m_width;
        int m_height;
        Texture* m_defaultTexture;
        Texture* m_currentTexture;
        Shader* m_defaultShader;
        Shader* m_currentShader;
        Camera* m_camera;
};

#endif // SCENEPARAMS_H__
