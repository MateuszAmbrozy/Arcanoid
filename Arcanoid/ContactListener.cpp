#include "stdafx.h"
#include "ContactListener.h"

ContactListener::ContactListener(b2World* world)
{
    this->world = world;
}

ContactListener::~ContactListener() {}

void ContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    //BallUserData* userDataA = reinterpret_cast<BallUserData*>(bodyA->GetUserData().pointer);
   // BallUserData* userDataB = reinterpret_cast<BallUserData*>(bodyB->GetUserData().pointer);

    UserData* userDataA = (UserData*)fixtureA->GetUserData().pointer;
    UserData* userDataB = (UserData*)fixtureB->GetUserData().pointer;
    if (userDataA && userDataB)
    {

       if (userDataA->name == "ball" && userDataB->name == "block")
       {
           userDataB->hit = true; 
       }
       if (userDataA->name == "block" && userDataB->name == "ball")
       {
           userDataA->hit = true;
       }
       if (userDataA->name == "paddle" && userDataB->name == "ball")
       {
           userDataB->hit = true;
       }
       if (userDataB->name == "paddle" && userDataA->name == "ball")
       {
           userDataA->hit = true;
       }

    }

}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    UserData* userDataA = (UserData*)fixtureA->GetUserData().pointer;
    UserData* userDataB = (UserData*)fixtureB->GetUserData().pointer;

    if (userDataA && userDataB)
    {
        if(userDataA->name == "ball" && userDataB->name == "ball")
             contact->SetEnabled(false);
    }
}
