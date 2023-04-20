// #include "Experiment.h"
#include "../util/Helper.h"
#include "../BoxCounting/Analysis.h"
#include <cstdlib>
#include <ctime>

#include <cmath>
#include <iostream>
#define M_PI 3.14159265358979323846

// Isotropy vs unisotropy
//
//
//  High Humidity(A = 10 looks good)
Experiment::Experiment(double humidity, double A, int gridSize, int spawnRadius) : humidity(humidity),
																				   A(A),
																				   m_gridSize(gridSize),
																				   m_frozenSiteCount(0),
																				   m_spawnRadius(spawnRadius),
																				   m_iterationCounter(0),
																				   m_walkerNucleusMap(nullptr)
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

	// Random Seed
	std::srand(std::time(0));
}

Experiment::~Experiment()
{
	for (int i = 0; i < m_gridSize; i++)
	{
		delete[] m_data[i];
	}
	delete[] m_data;

	if (m_walkerNucleusMap != nullptr)
		delete[] m_walkerNucleusMap;
}

void Experiment::Run(int maxFrozenSites, int snapshotInterval, int nuclei[], int nucleusCount)
{
	std::cout << "Started Simulation" << std::endl;

	// Flag tells us if the user gave us nuclei
	float pro = false;
	int nucleus[2] = {m_origin.x, m_origin.y};
	m_walkerNucleusMap = new int[m_maxWalkers];
	for (int i = 0; i < m_maxWalkers; i++)
		m_walkerNucleusMap[i] = -1;

	// Set Initial Frozen Sites
	if (nucleusCount == -1)
	{
		m_data[m_origin.y][m_origin.x] = FROZEN;
		nucleusCount = 1;
	}
	else
	{
		for (int i = 0; i < nucleusCount; i++)
		{
			if ((i * 2) >= 0 && (i * 2) < m_gridSize &&
				(i * 2 + 1) >= 0 && (i * 2 + 1) < m_gridSize)
			{
				std::cout << nuclei[i * 2 + 1] << " " << nuclei[i * 2] << std::endl;
				m_data[nuclei[i * 2 + 1]][nuclei[i * 2]] = FROZEN;
			}
		}
		pro = true;
	}

	int totalFrozenCount = 0;

	// Populate walkers
	int indexHelper = 0; // Helps us choose which nucleus to generate a walker for
	while (m_walkers.size() < (m_maxWalkers))
	{
		Vec2 walker = generateWalker();
		if (walker.x != -1)
		{
			// m_walkerNucleusMap[m_walkers.size()] = (pro) ? (indexHelper % nucleusCount) : 0;
			m_walkers.push_back(walker);
		}
		indexHelper++;
	}

	bool needToSave = false;
	int iterationsSinceLastStick = 0;
	while (totalFrozenCount < maxFrozenSites)
	{
		if (needToSave && totalFrozenCount % snapshotInterval == 0)
		{
			std::cout << totalFrozenCount << "/" << maxFrozenSites << std::endl;
			Helper::GenerateBitmap(this);
			needToSave = false;
		}

		if (iterationsSinceLastStick >= MAX_INTERATION_WITH_NO_STICK * nucleusCount)
		{
			std::cout << "Walkers did not stick!" << std::endl;
			break;
		}

		// Walk
		bool stuckOrFrozen = false;
		for (int i = m_walkers.size() - 1; i >= 0; i--)
		{
			randomWalk(m_walkers[i], i, nuclei);
			int status = walkerStatus(m_walkers[i]);
			m_data[m_walkers[i].y][m_walkers[i].x] = status;

			// If walker is not still walking, remove it from list.
			if (status != WET)
			{
				if (status == FROZEN)
				{
					totalFrozenCount++;
					needToSave = true;
				}

				if (m_walkers[i].distSqrd(m_origin) >= 0.8 * m_spawnRadius * m_spawnRadius)
					m_spawnRadius += 2;
				m_walkers.erase(m_walkers.begin() + i);

				// for (int l = i; l < m_maxWalkers - 1; l++) {
				// m_walkerNucleusMap[l] = m_walkerNucleusMap[l + 1];
				//}

				stuckOrFrozen = true;
			}
		}

		if (stuckOrFrozen)
			iterationsSinceLastStick = 0;
		else
			iterationsSinceLastStick++;

		// Create new walkers as needed
		while (m_walkers.size() < (m_maxWalkers))
		{
			Vec2 walker = generateWalker();
			if (walker.x != -1)
			{
				// m_walkerNucleusMap[m_walkers.size()] = (pro) ? (indexHelper % nucleusCount) : 0;
				m_walkers.push_back(walker);
				indexHelper++;
			}
		}
	}
	Helper::GenerateBitmap(this);
	std::cout << "Simulation Ended" << std::endl;

	double fd = AnalysisTools::CalculateFractalDimension(m_data, m_gridSize);
	std::cout << "Fractal Dimension: " << fd << std::endl;
}

void Experiment::randomWalk(Vec2 &walker, int walkerIndex, int *nuclei)
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
	} while ((walker.x + dir.x < 0 || walker.x + dir.x >= m_gridSize) ||
			 (walker.y + dir.y < 0 || walker.y + dir.y >= m_gridSize) ||
			 m_data[walker.y + dir.y][walker.x + dir.x] != WET); // Keep on generating until we land on wet spot

	walker.x += dir.x;
	walker.y += dir.y;

	// Constrain to spawn radius
	// int nucX = nuclei[m_walkerNucleusMap[walkerIndex] * 2];
	// int nucY = nuclei[m_walkerNucleusMap[walkerIndex] * 2+1];
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
		if (m_frozenSiteCount > 100) // TODO need to do something with multiple nucleus
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
	// Generate on Borders
	// Generate Along radius
	double angle = Helper::GenerateRandomVariable() * (M_PI * 2);
	int x = std::cos(angle) * m_spawnRadius + m_origin.x;
	int y = std::sin(angle) * m_spawnRadius + m_origin.y;

	// Realistically I need to check if the nucleus given wasnt skipped on initialization but it doesnt really matter for our purposes I think
	x = Helper::constrain(x, 0, m_gridSize - 1);
	y = Helper::constrain(y, 0, m_gridSize - 1);

	if (m_data[y][x] == WET)
		return {x, y};
	else
		return {-1, -1};
}
