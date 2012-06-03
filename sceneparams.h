#ifndef SCENEPARAMS_H__
#define SCENEPARAMS_H__

class SceneParams
{
    public:
        SceneParams();

        void SetWidth(int v);
        int GetWidth() const;
        void SetHeight(int v);
        int GetHeight() const;

    private:
        int m_width;
        int m_height;
};

#endif // SCENEPARAMS_H__
