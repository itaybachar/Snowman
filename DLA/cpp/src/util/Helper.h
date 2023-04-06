#pragma once
#include "../Experiment/Experiment.h"

namespace Helper
{
    bool hasHoles(Vec2 walker, Experiment *e);
    double FrostProbability(Vec2 walker, Experiment *e);
    double GenerateRandomVariable();
    int32_t constrain(int32_t val, int32_t min, int32_t max);
    void GenerateBitmap(Experiment *e);
};