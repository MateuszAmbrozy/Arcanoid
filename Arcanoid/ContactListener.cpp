#include "ContactListener.h"
#include "stdafx.h"

namespace
{
UserData* data(b2Fixture* fixture)
{
    return reinterpret_cast<UserData*>(fixture->GetUserData().pointer);
}
} // namespace

void ContactListener::BeginContact(b2Contact* contact)
{
    auto* a = data(contact->GetFixtureA());
    auto* b = data(contact->GetFixtureB());
    if (!a || !b)
        return;
    if (a->type == ObjectType::Ball && b->type == ObjectType::Block)
        b->hit = true;
    if (b->type == ObjectType::Ball && a->type == ObjectType::Block)
        a->hit = true;
    if (a->type == ObjectType::Ball && b->type == ObjectType::Paddle)
        a->hit = true;
    if (b->type == ObjectType::Ball && a->type == ObjectType::Paddle)
        b->hit = true;
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold*)
{
    const auto* a = data(contact->GetFixtureA());
    const auto* b = data(contact->GetFixtureB());
    if (a && b && a->type == ObjectType::Ball && b->type == ObjectType::Ball)
        contact->SetEnabled(false);
}
