#ifndef SCENEPARAMS_H__
#define SCENEPARAMS_H__

class Texture;
class Shader;
class Camera;

class SceneParams
{
    public:
        enum PARAM_VALUE { NOT_SET, ENABLED, DISABLED };

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

        void SetCurrentDepthTest(PARAM_VALUE depthTest);
        PARAM_VALUE GetCurrentDepthTest() const;

    private:
        int m_width;
        int m_height;
        Texture* m_defaultTexture;
        Texture* m_currentTexture;
        Shader* m_defaultShader;
        Shader* m_currentShader;
        Camera* m_camera;

        PARAM_VALUE m_depthTest;
};

#endif // SCENEPARAMS_H__
