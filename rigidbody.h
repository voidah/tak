#ifndef TAK_RIGIDBODY_H__
#define TAK_RIGIDBODY_H__

#include <btBulletDynamicsCommon.h>
#include "vector3.h"

class SceneNode;

class RigidBody
{
    public:
        RigidBody(float weight);
        virtual ~RigidBody();

        void SetPosition(const Vector3f& position);
        Vector3f GetPosition() const;

        Vector3f GetRotation() const;

        void SetLinearVelocity(const Vector3f& velocity);
        Vector3f GetLinearVelocity() const;

        void SetKinematic();

        void ApplyForce(const Vector3f& force);

        btRigidBody* GetInternalHandle() const;

        void LinkWithNode(SceneNode* node);
        SceneNode* GetLinkedNode() const;

    protected:
        void SetInternalHandle(btRigidBody* handle);

    private:
        btRigidBody* m_rigidBody;
        float m_weight;
        SceneNode* m_node;
};

class BoxRigidBody : public RigidBody
{
    public:
        BoxRigidBody(float weight, const Vector3f& position, const Vector3f& size);
        ~BoxRigidBody();
};

class CapsuleRigidBody : public RigidBody
{
    public:
        CapsuleRigidBody(float weight);
        ~CapsuleRigidBody();
};

#endif // TAK_RIGIDBODY_H__
