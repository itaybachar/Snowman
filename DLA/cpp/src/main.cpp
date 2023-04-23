#include <iostream>
#include <string>
#include "Experiment/Experiment.h"

int main(int argc, char *argv[])
{
    // Experiment Control
    int maxFrozenSites = 100000; // Stopping parameter, simulation will stop when this occurs
    int snapshotInterval = 1000; // How many walkers need to stick between frames

    // Experiment Parameters
    double humidity = 0.5f, A = 9, B = 0.5f;
    int gridSize = 1000;

    // Check if command line arguments were provided
    if (argc > 1)
    {
        humidity = std::stod(argv[1]);
        std::cout << "Humidity" << humidity << std::endl;
    }
    if (argc > 2)
    {
        A = std::stod(argv[2]);
    }
    if (argc > 3)
    {
        B = std::stod(argv[3]);
    }
    if (argc > 4)
    {
        gridSize = std::stoi(argv[4]);
        maxFrozenSites = (maxFrozenSites < gridSize * gridSize) ? maxFrozenSites : gridSize * gridSize * 0.8f;
    }

    // Initialize the Experiment
    Experiment dla(humidity, A, gridSize,15);

    // Run DLA
    std::cout << "Simulation Started" << std::endl;
    dla.Run(maxFrozenSites, snapshotInterval);
    std::cout << "Finished" << std::endl;
    // Save final growth, and export gif

    return 0;
}
extern "C" {
	__declspec(dllexport) int SimulateFrost(double humidity, double A, int gridSize, int maxFrozenSites, int snapshotInterval)
	{
		Experiment dla(humidity, A, gridSize,15);
		dla.Run(maxFrozenSites, snapshotInterval);
		return 0;
	}

	__declspec(dllexport) int SimulateFrostPro(double humidity, double A, int gridSize, int maxFrozenSites, int snapshotInterval, int nuclei[], int nucleusCount, int spawnRadius)
	{
		Experiment dla(humidity, A, gridSize, spawnRadius);
		dla.Run(maxFrozenSites, snapshotInterval,nuclei,nucleusCount);
		return 0;
	}
}