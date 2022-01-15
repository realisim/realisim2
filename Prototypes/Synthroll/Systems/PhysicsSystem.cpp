

#include "box2d/b2_circle_shape.h"
#include "box2d/b2_edge_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "Core/Logger.h"
#include "Systems/GameSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/SystemIds.h"

using namespace Realisim;
using namespace Math;
using namespace Reactor;
using namespace Synthroll;

namespace
{
    const float kTimeStep = 1.0f / 60.f;

    const int32 kVelocityIterations = 6;
    const int32 kPositionIterations = 2;
}

//---------------------------------------------------------------------------------------------------------------------
PhysicsSystem::PhysicsSystem(Reactor::Broker* ipBroker, Reactor::Hub* ipHub) : ISystem(ipBroker, ipHub),
mState(sUndefined),
mpWorld(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
PhysicsSystem::~PhysicsSystem()
{
    if (mpWorld) {
        delete mpWorld;
        mpWorld = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool PhysicsSystem::init()
{
    LOG_TRACE(Core::Logger::llNormal, "Initializing PhysicsSystem...");

    b2Vec2 gravity(0.0f, -9.8f);
    mpWorld = new b2World(gravity);

    setState(sRunning);
    LOG_TRACE(Core::Logger::llNormal, "\t done.");

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::removeSegment(int iIndex)
{
    auto it = mLineSegmentToBody.find(iIndex);
    if (it != mLineSegmentToBody.end())
    {
        b2Body* pBody = it->second;
        mpWorld->DestroyBody(pBody);

        mLineSegmentToBody.erase(it);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// clear the physics and reposition eveything as if the stage had just started
void PhysicsSystem::reset()
{
    mpWorld->ClearForces();

    // reposition the player
    const Math::Vector2 startPosition = mpTerrain->getStartPoint();
    mpPlayerBody->SetTransform(b2Vec2((float)startPosition.x(), (float)startPosition.y()), 0.f);
    mpPlayerBody->SetLinearVelocity(b2Vec2(0.f, 0.f));
    mpPlayerBody->SetAngularVelocity(0.0f);
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::setState(State iV)
{
    switch (iV)
    {
    case PhysicsSystem::sUndefined:
        assert(false && "this should not be done...");
        break;
    case PhysicsSystem::sRunning: mState = iV; break;
    case PhysicsSystem::sPaused: mState = iV; break;
    default: assert(false); break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::setTerrain(const Terrain* ipTerrain)
{
    mpTerrain = ipTerrain;

    // flush all bodys...
    if (mpPlayerBody) {
        mpWorld->DestroyBody(mpPlayerBody);
        mpPlayerBody = nullptr;
    }

    // create required bodys for this terrain
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystem(GAME_SYSTEM_ID);
    Player& p = gs->getPlayerRef();

    const Math::Vector2 startPosition = mpTerrain->getStartPoint();
    // create dynamic moving object.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set((float)startPosition.x(), (float)startPosition.y());
    mpPlayerBody = mpWorld->CreateBody(&bodyDef);

    b2CircleShape dynamicCircle;
    dynamicCircle.m_radius = p.getRadius();

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;

    mpPlayerBody->CreateFixture(&fixtureDef);
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::update()
{
    if(getState() == sRunning)
        mpWorld->Step(kTimeStep, kVelocityIterations, kPositionIterations);
    
    //--- update player
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystem(GAME_SYSTEM_ID);
    Player& p = gs->getPlayerRef();

    b2Vec2 position = mpPlayerBody->GetPosition();
    //float angle = mpDynamicBody->GetAngle();

    ModelNode& mn = p.getModelNodeRef();
    Matrix4 t;
    t.setAsTranslation(Vector3(position.x, position.y, 0));
    mn.setParentTransform(t);

    updateTerrain();
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::updateTerrain()
{
    if (!mpTerrain) return;

    if (mpTerrain->isDirty())
    {
        // update all dirty terrain segments
        const int numSegments = mpTerrain->getNumberOfSegments();
        for (int i = 0; i < numSegments; ++i)
        {
            const Geometry::LineSegment& l = mpTerrain->getSegment(i);
            if (mpTerrain->isSegmentDirty(i))
            {
                removeSegment(i);
                
                // add segment
                b2BodyDef lineDef;
                lineDef.position.Set(0, 0);
                lineDef.angle = 0.0f;
                b2Body* pLineBody = mpWorld->CreateBody(&lineDef);
                
                b2EdgeShape lineShape;
                b2Vec2 a((float)l.mP0.x(), (float)l.mP0.y());
                b2Vec2 b((float)l.mP1.x(), (float)l.mP1.y());
                lineShape.SetTwoSided(a, b);
                pLineBody->CreateFixture(&lineShape, 0.0f);

                mLineSegmentToBody[i] = pLineBody;
            }
        }
    }
}