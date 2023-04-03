#include "Experiment.h"

Experiment::Experiment(float humidity, float A, float B, uint32_t gridSize) : m_humidity(humidity),
                                                                              m_A(A),
                                                                              m_B(B),
                                                                              m_gridSize(gridSize)
{
    m_data = new uint8_t *[m_gridSize];
    for (uint32_t i = 0; i < m_gridSize; i++)
    {
        m_data[i] = new uint8_t[m_gridSize];
        for (uint32_t j = 0; j < m_gridSize; j++)
            m_data[i][j] = WET;
    }

    m_origin = {m_gridSize / 2,
                m_gridSize / 2};

    m_data[m_origin.y][m_origin.x] = FROZEN;
}

Experiment::~Experiment()
{
    for (uint32_t i = 0; i < m_gridSize; i++)
    {
        delete[] m_data[i];
    }
    delete[] m_data;
}

void Experiment::Run(uint32_t maxFrozenSites, uint32_t snapshotInterval)
{
    uint32_t totalFrozenCount = 0;

    while (totalFrozenCount < maxFrozenSites)
    {
        if (totalFrozenCount % snapshotInterval == 0)
        {
            // Save Current State
        }

        // Walk
    }
}

void Experiment::SaveResults()
{
}