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
    UserData* userDataA = (UserData*)contact->GetFixtureA()->GetUserData().pointer;
    UserData* userDataB = (UserData*)contact->GetFixtureB()->GetUserData().pointer;

    if (userDataA && userDataB)
    {
        if(userDataA->name == "ball" && userDataB->name == "ball")
             contact->SetEnabled(false);
    }
}
