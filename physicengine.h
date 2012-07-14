#ifndef TAK_PHYSICENGINE_H__
#define TAK_PHYSICENGINE_H__

#include <btBulletDynamicsCommon.h>
#include "rigidbody.h"
#include "gldebugdrawer.h"

#define GPhysicEngine (*(PhysicEngine::Instance()))

// Singleton
class PhysicEngine
{
    public:
        static PhysicEngine* Instance()
        {
            static PhysicEngine inst;
            return &inst;
        }

        void Init();
        void DeInit();

        btCollisionWorld* GetCollisionWorld();
        btDynamicsWorld* GetDynamicsWorld();
        btBroadphaseInterface* GetBroadphase();

        void AddRigidBody(RigidBody* rigidBody);
        void RemoveRigidBody(RigidBody* rigidBody);

        void AddAction(btActionInterface* action);
        void RemoveAction(btActionInterface* action);

        void DrawDebug() const;

        void Update(float elapsedTime);

    private:
        PhysicEngine();
        ~PhysicEngine();
        PhysicEngine(const PhysicEngine&) {}
        PhysicEngine* operator=(const PhysicEngine&) {}

    private:
        bool m_isValid;

        btDiscreteDynamicsWorld* m_dynamicsWorld;
        btSequentialImpulseConstraintSolver* m_solver;
        btCollisionDispatcher* m_dispatcher;
        btDefaultCollisionConfiguration* m_collisionConfiguration;
        btBroadphaseInterface* m_broadphase;

        GLDebugDrawer m_debugDrawer;
};

#endif // TAK_PHYSICENGINE_H__
