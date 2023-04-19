#include "Analysis.h"
#include <vector>
#include <cmath>
#include <iostream>

namespace AnalysisTools
{

    namespace
    {
        int countBoxes(std::vector<std::vector<int>> data, int gridSize, int boxSize)
        {
            int count = 0;
            int rows = gridSize;
            int cols = gridSize;
            for (int i = 0; i < rows; i += boxSize)
            {
                for (int j = 0; j < cols; j += boxSize)
                {
                    bool found = false;
                    for (int k = i; k < i + boxSize; k++)
                    {
                        for (int l = j; l < j + boxSize; l++)
                        {
                            if (k < rows && l < cols && data[k][l] != 0)
                            {
                                found = true;
                                break;
                            }
                        }
                        if (found)
                        {
                            break;
                        }
                    }
                    if (found)
                    {
                        count++;
                    }
                }
            }
            return count;
        }
    };
    float CalculateFractalDimension(std::vector<std::vector<int>> data, int gridSize)
    {
        std::vector<int> boxCounts;
        for (int boxSize = 1; boxSize <= gridSize; boxSize++)
        {
            int count = countBoxes(data, gridSize, boxSize);
            boxCounts.push_back(count);
        }

        // calculate the fractal dimension using linear regression
        double logBoxSizeSum = 0;
        double logCountSum = 0;
        int n = boxCounts.size();
        for (int i = 0; i < n; i++)
        {
            double logBoxSize = log(i + 1);
            double logCount = log(boxCounts[i]);
            logBoxSizeSum += logBoxSize;
            logCountSum += logCount;
        }

        double logBoxSizeMean = logBoxSizeSum / n;
        double logCountMean = logCountSum / n;
        double numerator = 0;
        for (int i = 0; i < n; i++)
        {
            double logBoxSize = log(i + 1);
            double logCount = log(boxCounts[i]);
            numerator += (logBoxSize - logBoxSizeMean) * (logCount - logCountMean);
        }

        double denominator = 0;
        for (int i = 0; i < n; i++)
        {
            double logBoxSize = log(i + 1);
            denominator += pow(logBoxSize - logBoxSizeMean, 2);
        }
        double slope = numerator / denominator;
        double fractalDimension = -slope;

        return fractalDimension;
    }
};

int main()
{
    std::vector<std::vector<int>> grid = {
        {1, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 1, 1, 1}};

    std::cout << "HERE" << std::endl;
    std::cout << AnalysisTools::CalculateFractalDimension(grid, grid.size());
    return 0;
}