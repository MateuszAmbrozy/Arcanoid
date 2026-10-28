#pragma once
#include "stdafx.h"

enum class ObjectType
{
    Ball,
    Block,
    Paddle
};

struct UserData
{
    ObjectType type = ObjectType::Ball;
    bool hit = false;
};
