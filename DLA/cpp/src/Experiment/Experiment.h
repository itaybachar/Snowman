#pragma once
#include "../util/Vec2.h"
#include <vector>

class Experiment
{
public:
    Experiment(double humidity, double A, double B, int gridSize);
    ~Experiment();

    void Run(int maxFrozenSites = 1000, int snapshotInterval = 50);
    void SaveResults();

    inline int GetGridSize() { return m_gridSize; }
    inline int **GetData() { return m_data; }

public:
    double humidity;
	double A;
	double B;
    static constexpr int WET = 0;
    static constexpr int DRY = 1;
    static constexpr int FROZEN = 2;

private:
    // Experiment Setup
    int m_gridSize;
    int m_frozenSiteCount;
	const int m_maxWalkers = 50;
    int m_spawnRadius;

    // Experiment Variables
    int **m_data;
    int m_iterationCounter;
    std::vector<Vec2> m_walkers;
    Vec2 m_origin;
	static constexpr int MAX_INTERATION_WITH_NO_STICK = 20000;

private:
    void randomWalk(Vec2 &walker);
    int walkerStatus(Vec2 walker);
    Vec2 generateWalker();
};