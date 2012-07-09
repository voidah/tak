#ifndef TAK_PLAYER_H__
#define TAK_PLAYER_H__

#include <string>
#include "define.h"
#include "vector3.h"
#include "rigidbody.h"
#include "physicengine.h"

#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

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

        void Move(bool front, bool back, bool left, bool right, bool run, bool fly, float elapsedTime);

        bool IsOnGround() const;
        void Jump();

        void SetPosition(const Vector3f& position);
        Vector3f GetPosition() const;

        Vector3f GetEyePosition() const;

        void Rotate(const Vector3f& deltaRotation);
        void SetRotation(const Vector3f& rotation);
        const Vector3f& GetRotation() const;

        Vector3f GetEyeVector() const;

    private:
        class MotionState : public btMotionState
    {
        public:
            MotionState(Player* player) : m_player(player)
        {
            //m_pos.setIdentity();
        }

            virtual ~MotionState() {}

            virtual void getWorldTransform(btTransform &worldTrans) const
            {
                Vector3f position = m_player->GetPosition();
                Vector3f rotation = m_player->GetRotation();
                rotation.x = rotation.y = rotation.z = 0;
                worldTrans = btTransform(btQuaternion(rotation.z, rotation.y, rotation.x), btVector3(position.x, position.y, position.z));
            }

            virtual void setWorldTransform(const btTransform &worldTrans)
            {
                // Rotation
                float x, y, z;
                worldTrans.getBasis().getEulerZYX(x, y, z);
                m_player->SetRotation(Vector3f(RADTODEG(x), RADTODEG(y), RADTODEG(z)));

                // Position
                btVector3 pos = worldTrans.getOrigin();
                m_player->SetPosition(Vector3f(pos.x(), pos.y(), pos.z()));
            }

        private:
            Player* m_player;
    };


        class PlayerRigidBody : public RigidBody
    {
        public:
            PlayerRigidBody(Player* player, MotionState* motionState) : RigidBody(0)
        {
            // The total height is height+2*radius
            // TODO set according to player height
            m_shape = new btCapsuleShape(.25f, player->GetSize().y - .5f);

            // TODO I think kinematic body should have a mass of 0
            //btScalar mass = player->GetWeight();
            btScalar mass = 0;

            btVector3 fallInertia(0,0,0);
            m_shape->calculateLocalInertia(mass,fallInertia);
            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,motionState,m_shape,fallInertia);
            m_rigidBody = new btRigidBody(fallRigidBodyCI);


            m_rigidBody->setSleepingThresholds(0, 0);
            m_rigidBody->setAngularFactor(0);
            SetKinematic();
        }

            virtual ~PlayerRigidBody()
            {
                delete m_shape;
            }

            btConvexShape* GetCollisionShape()
            {
                return m_shape;
            }

        private:
            btConvexShape* m_shape;
    };

        class CollisionList : public btCollisionWorld::ConvexResultCallback
    {
        public:
            struct Collision
            {
                Vector3f hitPoint;

                Collision(const Vector3f& nHitPoint) : hitPoint(nHitPoint) {}
            };

            typedef std::vector<Collision> Collisions;
            typedef Collisions::const_iterator ConstIterator;

        public:
            CollisionList()
            {
                m_collisionFilterMask = btBroadphaseProxy::StaticFilter;
            }

            ConstIterator Begin() const
            {
                return m_collisions.begin();
            }

            ConstIterator End() const
            {
                return m_collisions.end();
            }

            int Count() const
            {
                return m_collisions.size();
            }

            const Collisions& GetCollisions() const
            {
                return m_collisions;
            }

        private:
            virtual btScalar addSingleResult (btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace)
            {
                if(convexResult.m_hitFraction != 0)
                {
                    //std::cout << "Collision detected!" << std::endl;
                    btVector3& hitPoint = convexResult.m_hitPointLocal;
                    //std::cout << hitPoint.x() << ", " << hitPoint.y() << ", " << hitPoint.z() << std::endl;
                    AddCollision(Vector3f(hitPoint.x(), hitPoint.y(), hitPoint.z()));
                }

                // To verify: it seems bullet does'nt care about the return value
                return 0;
            }

            void AddCollision(const Vector3f& hitPoint)
            {
                // Small optimisation, allocate more space now to prevent multiple
                // subsequent re-allocation from the vector
                if(m_collisions.size() == 0)
                    m_collisions.reserve(8);

                m_collisions.push_back(Collision(hitPoint));
            }

        private:
            Collisions m_collisions;

    };

        MotionState* m_motion;
        PlayerRigidBody* m_rigidBody;

    private:
        void ClampRotation();

    private:
        std::string m_name;
        Vector3f m_size;
        float m_weight;
        float m_eyeHeight; // From foot
        Vector3f m_position;
        Vector3f m_rotation;

        btKinematicCharacterController* m_character;
        btPairCachingGhostObject* m_ghostObject;

};

#endif // TAK_PLAYER_H__
