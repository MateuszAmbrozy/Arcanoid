#pragma once
#include "ObjectData.h"

class ContactListener : public b2ContactListener
{
private:
    b2World* world;
    

public:
    virtual void BeginContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);


public:
    ~ContactListener();
    ContactListener(b2World* world);
};

