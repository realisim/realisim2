

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
mpWorld(nullptr),
mCurrentRecordIndex(0),
mLastRecordIndex(0)
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
void PhysicsSystem::forward(int iIncrement)
{
    if (getState() != sPaused) return;

    const int index = std::min(mCurrentRecordIndex + iIncrement, (int)mLastRecordIndex - 1);
    if (index <= mLastRecordIndex) {
        mCurrentRecordIndex = index;

        const ReplayRecord& rr = mPlayerReplayRecords[mCurrentRecordIndex];
        mpPlayerBody->SetTransform(rr.mPosition, rr.mAngle);
        mpPlayerBody->SetLinearVelocity(rr.mLinearVelocity);
        mpPlayerBody->SetAngularVelocity(rr.mAngularVelocity);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool PhysicsSystem::init()
{
    LOG_TRACE(Core::Logger::llNormal, "Initializing PhysicsSystem...");

    b2Vec2 gravity(0.0f, -9.8f);
    mpWorld = new b2World(gravity);

    // 1 minute buffer
    mPlayerReplayRecords.resize(60 * 60);
    mCurrentRecordIndex = 0;
    mLastRecordIndex = 0;

    setState(sRunning);
    LOG_TRACE(Core::Logger::llNormal, "\t done.");

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::removeSpline(int iIndex)
{
    auto it = mSplineToBody.find(iIndex);
    if (it != mSplineToBody.end())
    {
        for (auto pBody : it->second) {
            mpWorld->DestroyBody(pBody);
        }

        mSplineToBody.erase(it);
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

    // clear the replay
    mCurrentRecordIndex = 0;
    mLastRecordIndex = 0;
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::resume() 
{ 
    mLastRecordIndex = mCurrentRecordIndex;
    setState(sRunning);
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::rewind(int iIncrement)
{
    if (getState() != sPaused) return;

    const int index = std::max(mCurrentRecordIndex - iIncrement, 0);
    if (index <= mLastRecordIndex) {
        mCurrentRecordIndex = index;

        const ReplayRecord& rr = mPlayerReplayRecords[mCurrentRecordIndex];
        mpPlayerBody->SetTransform(rr.mPosition, rr.mAngle);
        mpPlayerBody->SetLinearVelocity(rr.mLinearVelocity);
        mpPlayerBody->SetAngularVelocity(rr.mAngularVelocity);
    }
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
    fixtureDef.density = 3.0f;
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.5f;

    mpPlayerBody->CreateFixture(&fixtureDef);
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::update()
{
    GameSystem* gs = (GameSystem*)getHubRef().getUserDefinedSystem(GAME_SYSTEM_ID);
    Player& p = gs->getPlayerRef();
   
    ReplayRecord rr;
    rr.mPosition = mpPlayerBody->GetPosition();
    rr.mAngularVelocity = mpPlayerBody->GetAngularVelocity();
    rr.mLinearVelocity = mpPlayerBody->GetLinearVelocity();
    rr.mAngle = mpPlayerBody->GetAngle();

    if (getState() == sRunning) {
        mpWorld->Step(kTimeStep, kVelocityIterations, kPositionIterations);

        mPlayerReplayRecords[mLastRecordIndex] = rr;
        mLastRecordIndex++;
        mCurrentRecordIndex++;
        if (mLastRecordIndex >= mPlayerReplayRecords.size())
            mPlayerReplayRecords.resize(size_t(mPlayerReplayRecords.size() * 1.2));
    }
    

    ModelNode& mn = p.getModelNodeRef();
    Matrix4 t, rTextureAlignment, rAngle;
    t.setAsTranslation(Vector3(rr.mPosition.x, rr.mPosition.y, 0));
    rTextureAlignment.setAsRotation(M_PI, Vector3(0, 1, 0));
    rAngle.setAsRotation(rr.mAngle, Vector3(0, 0, 1));
    mn.setParentTransform(t * rAngle * rTextureAlignment);

    updateTerrain();
}

//---------------------------------------------------------------------------------------------------------------------
void PhysicsSystem::updateTerrain()
{
    if (!mpTerrain) return;

    if (mpTerrain->isDirty())
    {
        // update all dirty terrain splines
        const int numSplines = mpTerrain->getNumberOfSplines();
        for (int i = 0; i < numSplines; ++i)
        {
            if (mpTerrain->isSplineDirty(i)) {
                removeSpline(i);

                const int numPolygons = mpTerrain->getNumberOf2dPolygons(i);
                for (int j = 0; j < numPolygons; ++j)
                {
                    const Polygon2d& p2d = mpTerrain->get2dPolygon(i, j);

                    b2BodyDef polygonDef;
                    polygonDef.position.Set(0, 0);
                    polygonDef.angle = 0.0f;
                    b2Body* pPolygonBody = mpWorld->CreateBody(&polygonDef);

                    const std::vector<Math::Vector2>& vertices = p2d.getVertices();
                    b2Vec2 *b2Vertices = new b2Vec2[p2d.getNumberOfVertices()];
                    for (int k = 0; k < p2d.getNumberOfVertices(); ++k) {
                        b2Vertices[k] = b2Vec2((float)vertices[k].x(), (float)vertices[k].y());
                    }

                    b2PolygonShape polyShape;
                    polyShape.Set(b2Vertices, p2d.getNumberOfVertices());

                    pPolygonBody->CreateFixture(&polyShape, 0.0f);

                    mSplineToBody[i].push_back(pPolygonBody);
                }
            }
        }
    }
}