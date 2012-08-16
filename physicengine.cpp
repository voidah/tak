#include "physicengine.h"
#include <cassert>

PhysicEngine::PhysicEngine() : m_isValid(false)
{
}

PhysicEngine::~PhysicEngine()
{
}

void PhysicEngine::AddRigidBody(RigidBody* rigidBody)
{
    assert(m_isValid);
    m_dynamicsWorld->addRigidBody(rigidBody->GetInternalHandle());
}

void PhysicEngine::RemoveRigidBody(RigidBody* rigidBody)
{
    assert(m_isValid);
    m_dynamicsWorld->removeRigidBody(rigidBody->GetInternalHandle());
}

void PhysicEngine::AddAction(btActionInterface* action)
{
    assert(m_isValid);
    m_dynamicsWorld->addAction(action);
}

void PhysicEngine::RemoveAction(btActionInterface* action)
{
    assert(m_isValid);
    m_dynamicsWorld->removeAction(action);
}

void PhysicEngine::DrawDebug() const
{
    if(m_isValid)
        m_dynamicsWorld->debugDrawWorld();
}

void PhysicEngine::Update(float elapsedTime)
{
    assert(m_isValid);
    m_dynamicsWorld->stepSimulation(elapsedTime, 10);
}

bool PhysicEngine::RayCastClosestCollisionPoint(const Vector3f& from, const Vector3f& to, Vector3f& hitPoint, RigidBody*& hitBody) const
{
    btVector3 btFrom = btVector3(from.x, from.y, from.z);
    btVector3 btTo = btVector3(to.x, to.y, to.z);
    btCollisionWorld::ClosestRayResultCallback rayCallback(btFrom, btTo);
    m_dynamicsWorld->rayTest(btFrom, btTo, rayCallback);

    if(rayCallback.hasHit())
    {
        btRigidBody* pBody = btRigidBody::upcast(rayCallback.m_collisionObject);

        hitBody = 0;
        if(pBody)
            hitBody = (RigidBody*)pBody->getUserPointer();

        btVector3& hit = rayCallback.m_hitPointWorld;
        hitPoint.x = hit.x();
        hitPoint.y = hit.y();
        hitPoint.z = hit.z();
        return true;
    }

    return false;
}

void PhysicEngine::Init()
{
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0,-10,0));

    // TODO
    //m_dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;

    // TODO
    m_debugDrawer.setDebugMode(btIDebugDraw::DBG_NoDebug);
    //m_debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawContactPoints);
    m_dynamicsWorld->setDebugDrawer(&m_debugDrawer);

    m_isValid = true;
}

void PhysicEngine::DeInit()
{
    assert(m_isValid);
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_dispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

btCollisionWorld* PhysicEngine::GetCollisionWorld()
{
    return m_dynamicsWorld;
}

btDynamicsWorld* PhysicEngine::GetDynamicsWorld()
{
    return m_dynamicsWorld;
}

btBroadphaseInterface* PhysicEngine::GetBroadphase()
{
    return m_broadphase;
}


