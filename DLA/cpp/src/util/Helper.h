#pragma once
#include "../Experiment/Experiment.h"

namespace Helper
{
    bool hasHoles(Vec2 walker, Experiment *e);
    double FrostProbability(Vec2 walker, Experiment *e);
    double GenerateRandomVariable();
    int constrain(int val, int min, int max);
    void GenerateBitmap(Experiment *e);
};