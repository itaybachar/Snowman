#pragma once
#include "../util/Util.h"
#include <vector>

class Experiment
{
public:
    Experiment(float humidity, float A, float B, uint32_t gridSize);
    ~Experiment();

    void Run(uint32_t maxFrozenSites = 1000, uint32_t snapshotInterval = 50);
    void SaveResults();

private:
    // Experiment Setup
    uint32_t m_gridSize;

    float m_humidity;
    float m_A;
    float m_B;

    // Experiment Variables
    uint8_t **m_data;
    uint64_t m_iterationCounter;
    std::vector<Vec2> m_walkers;
    Vec2 m_origin;

    const uint8_t WET = 0;
    const uint8_t DRY = 1;
    const uint8_t FROZEN = 2;
};