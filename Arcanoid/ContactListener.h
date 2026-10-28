#pragma once
#include "ObjectData.h"

class ContactListener final : public b2ContactListener
{
  public:
    void BeginContact(b2Contact* contact) override;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
};
