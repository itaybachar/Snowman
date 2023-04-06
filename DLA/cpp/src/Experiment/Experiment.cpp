// #include "Experiment.h"
#include "../util/Helper.h"
#include <cstdlib>
#include <ctime>

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

// Isotropy vs unisotropy
//
//
//  High Humidity(A = 10 looks good)
Experiment::Experiment(double humidity, float A, float B, int gridSize) : humidity(humidity),
                                                                              A(A),
                                                                              B(B),
                                                                              m_gridSize(gridSize),
                                                                              m_frozenSiteCount(0),
                                                                              m_spawnRadius(5),
	m_iterationCounter(0)
{
    m_data = new int *[m_gridSize];
    for (int i = 0; i < m_gridSize; i++)
    {
        m_data[i] = new int[m_gridSize];
        for (int j = 0; j < m_gridSize; j++)
            m_data[i][j] = WET;
    }

    m_origin = {m_gridSize / 2,
                m_gridSize / 2};

    m_data[m_origin.y][m_origin.x] = FROZEN;

    // Random Seed
    std::srand(std::time(0));

    //Helper::GenerateBitmap(this);
}

Experiment::~Experiment()
{
    for (int i = 0; i < m_gridSize; i++)
    {
        delete[] m_data[i];
    }
    delete[] m_data;
}

void Experiment::Run(int maxFrozenSites, int snapshotInterval)
{
    int totalFrozenCount = 0;

    // Populate walkers
    while (m_walkers.size() < m_maxWalkers)
    {
        m_walkers.push_back(generateWalker());
    }

    while (totalFrozenCount < maxFrozenSites)
    {
        if (totalFrozenCount % snapshotInterval == 0)
        {
            // Save Current State
        }

        // Walk
        for (int i = m_walkers.size() - 1; i >= 0; i--)
        {
            randomWalk(m_walkers[i]);
            int status = walkerStatus(m_walkers[i]);
            m_data[m_walkers[i].y][m_walkers[i].x] = status;
			std::cout << m_walkers[i].x << " " << m_walkers[i].y << std::endl;
            // If walker is not still walking, remove it from list.
            if (status != WET)
            {
                std::cout << "Stuck: " << (int)status << std::endl;
                Helper::GenerateBitmap(this);
                if (m_walkers[i].distSqrd(m_origin) >= 0.8 * m_spawnRadius * m_spawnRadius)
                    m_spawnRadius = m_spawnRadius++;
                m_walkers.erase(m_walkers.begin() + i);
            }
        }
        // std::cout << "walk" << std::endl;

        // Create new walkers as needed
        while (m_walkers.size() < m_maxWalkers)
            m_walkers.push_back(generateWalker());
    }
}

void Experiment::SaveResults()
{
}

void Experiment::randomWalk(Vec2 &walker)
{
    int directions[] = {-1, 0, 1};
    Vec2 dir;
    int attemptCounter = 0;
    do
    {
        dir.x = directions[rand() % 3];
        dir.y = directions[rand() % 3];
        if (attemptCounter++ > 1000)
            return;
    } while ((walker.x + dir.x < 0 && walker.x + dir.x >= m_gridSize) &&
             (walker.y + dir.y < 0 && walker.y + dir.y >= m_gridSize) &&
             m_data[walker.y + dir.y][walker.x + dir.x] != WET); // Keep on generating until we land on wet spot
    walker.x += dir.x;
    walker.y += dir.y;

    // Constrain to spawn radius
    walker.x = Helper::constrain(walker.x, m_origin.x - m_spawnRadius, m_origin.x + m_spawnRadius);
    walker.y = Helper::constrain(walker.y, m_origin.y - m_spawnRadius, m_origin.y + m_spawnRadius);

    // Constrain To Grid:
    walker.x = Helper::constrain(walker.x, 0, m_gridSize - 1);
    walker.y = Helper::constrain(walker.y, 0, m_gridSize - 1);
}

int Experiment::walkerStatus(Vec2 walker)
{
    bool hasNeighbor = false;
    if (walker.y + 1 < m_gridSize && m_data[walker.y + 1][walker.x] == FROZEN)
        hasNeighbor = true;

    if (walker.y - 1 >= 0 && m_data[walker.y - 1][walker.x] == FROZEN)
        hasNeighbor = true;

    if (walker.x + 1 < m_gridSize && m_data[walker.y][walker.x + 1] == FROZEN)
        hasNeighbor = true;

    if (walker.x - 1 >= 0 && m_data[walker.y][walker.x - 1] == FROZEN)
        hasNeighbor = true;

    if (!hasNeighbor)
        return WET;
    // If position would cause a hole, continue walking.
    if (Helper::hasHoles(walker, this))
        return WET;

    double p = Helper::FrostProbability(walker, this);

    if (Helper::GenerateRandomVariable() < p)
    {
        m_frozenSiteCount++;
        return FROZEN;
    }
    else
    {
        // Make sure we have a large enough frozen neuclus before we start drying
        if (m_frozenSiteCount > 100)
        {
            // Calculate Potential Drying
            if (Helper::GenerateRandomVariable() < (1.0 - humidity) * p)
                return DRY;
        }
        return WET;
    }
}

Vec2 Experiment::generateWalker()
{
    double angle = Helper::GenerateRandomVariable() * (3.14 * 2);
    int x = std::cos(angle) * m_spawnRadius + m_origin.x;
    int y = std::sin(angle) * m_spawnRadius + m_origin.y;

    x = Helper::constrain(x, 0, m_gridSize - 1);
    y = Helper::constrain(y, 0, m_gridSize - 1);
    return {x, y};
}
