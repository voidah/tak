#ifndef TAK_PLAYER_H__
#define TAK_PLAYER_H__

#include <string>
#include "define.h"
#include "vector3.h"
#include <string>

class Player
{
    public:
        Player(const std::string& name, const Vector3f& size, float weightKg, float eyeHeight);
        ~Player();

        void SetName(const std::string& name);
        const std::string& GetName() const;

        void SetSize(const Vector3f& size);
        const Vector3f& GetSize() const;

        void SetWeight(float weight);
        float GetWeight() const;

        void SetEyeHeight(float eyeHeight);
        float GetEyeHeight() const;

        Vector3f SimulateMove(bool front, bool back, bool left, bool right, bool run, bool fly, float elapsedTime);

        void SetPosition(const Vector3f& position);
        Vector3f GetPosition() const;

        Vector3f GetEyePosition() const;

        void Rotate(const Vector3f& deltaRotation);
        void SetRotation(const Vector3f& rotation);
        const Vector3f& GetRotation() const;

        Vector3f GetEyeVector() const;

    private:
        void ClampRotation();

    private:
        std::string m_name;
        Vector3f m_size;
        float m_weight;
        float m_eyeHeight; // From foot
        Vector3f m_position;
        Vector3f m_rotation;
};

#endif // TAK_PLAYER_H__
