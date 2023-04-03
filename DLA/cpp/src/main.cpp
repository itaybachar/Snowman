#include <iostream>
#include <string>
#include "Experiment/Experiment.h"

int main(int argc, char *argv[])
{
    // Experiment Control
    int maxFrozenSites = 75000; // Stopping parameter, simulation will stop when this occurs
    int snapshotInterval = 50;  // How many walkers need to stick between frames

    // Experiment Parameters
    float humidity = 0.5f, A = 10, B = 0.4f;
    int gridSize = 1000;

    // Check if command line arguments were provided
    if (argc > 1)
    {
        humidity = std::stof(argv[1]);
    }
    if (argc > 2)
    {
        A = std::stof(argv[2]);
    }
    if (argc > 3)
    {
        B = std::stof(argv[3]);
    }
    if (argc > 4)
    {
        gridSize = std::stoi(argv[4]);
        maxFrozenSites = (maxFrozenSites < gridSize * gridSize) ? maxFrozenSites : gridSize * gridSize * 0.8f;
    }

    // Initialize the Experiment
    Experiment dla(humidity, A, B, gridSize);

    // Run DLA
    dla.Run(maxFrozenSites, snapshotInterval);

    // Save final growth, and export gif
    dla.SaveResults();

    return 0;
}
