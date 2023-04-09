#pragma once
#include <stdint.h>

struct Vec2
{
    int x;
    int y;

    int distSqrd(Vec2 other)
    {
        return (other.x - this->x) * (other.x - this->x) + (other.y - this->y) * (other.y - this->y);
    }
};
