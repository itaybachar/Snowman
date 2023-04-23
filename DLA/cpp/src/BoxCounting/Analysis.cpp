#include "Analysis.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>
#include "../bmp/libbmp.h"

namespace AnalysisTools
{

    namespace
    {
        int countBoxes(int **data, int gridSize, int boxSize)
        {
            int count = 0;

            for (int i = 0; i < gridSize; i += boxSize)
            {
                for (int j = 0; j < gridSize; j += boxSize)
                {
                    bool found = false;
                    for (int k = i; k < i + boxSize; k++)
                    {
                        for (int l = j; l < j + boxSize; l++)
                        {
                            if (k < gridSize && l < gridSize && data[k][l] == 2)
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

        std::pair<double, double> linear_fit(std::vector<double> &x, std::vector<double> &y)
        {
            // calculate the means of x and y
            double mean_x = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
            double mean_y = std::accumulate(y.begin(), y.end(), 0.0) / y.size();
            // calculate the slope and coefficient
            double numerator = 0.0;
            double denominator = 0.0;
            for (int i = 0; i < x.size(); i++)
            {
                numerator += (x[i] - mean_x) * (y[i] - mean_y);
                denominator += (x[i] - mean_x) * (x[i] - mean_x);
            }
            double slope = numerator / denominator;
            double intercept = mean_y - slope * mean_x;

            return std::make_pair(slope, intercept);
        }
    };

    double CalculateFractalDimension(int **data, int gridSize)
    {
        // Find the maximum box size that can be used
        int max_box_size = ceil(log(gridSize) / log(2));

        // Iterate through all possible box sizes and count the number of boxes
        std::vector<double> boxSize;
        std::vector<double> count;
        for (int i = 0; i <= max_box_size; i++)
        {
            int box_size = pow(2, i);
            int boxes = countBoxes(data, gridSize, box_size);
            boxSize.push_back(i);
            count.push_back(log2((double)boxes));
        }

        // Calculate the fractal dimension
        auto out = linear_fit(boxSize, count);
        return -out.first;
    }

    // x,y = 0 as starter values
    void sierpinski_carpet(int **grid, int x, int y, int size)
    {
        if (size < 1)
        {
            return;
        }

        int subsize = size / 3;
        for (int i = x + subsize; i < x + 2 * subsize; i++)
        {
            for (int j = y + subsize; j < y + 2 * subsize; j++)
            {
                grid[i][j] = 1;
            }
        }

        sierpinski_carpet(grid, x, y, subsize);
        sierpinski_carpet(grid, x + subsize, y, subsize);
        sierpinski_carpet(grid, x + 2 * subsize, y, subsize);
        sierpinski_carpet(grid, x, y + subsize, subsize);
        sierpinski_carpet(grid, x + 2 * subsize, y + subsize, subsize);
        sierpinski_carpet(grid, x, y + 2 * subsize, subsize);
        sierpinski_carpet(grid, x + subsize, y + 2 * subsize, subsize);
        sierpinski_carpet(grid, x + 2 * subsize, y + 2 * subsize, subsize);
    }
};
// int main()
// {
//     int size = 512;
//     std::vector<std::vector<int>> grid(size, std::vector<int>(size, 0));
//     AnalysisTools::sierpinski_carpet(grid, 0, 0, size);

//     BmpImg img(size, size);

//     for (int y = 0; y < size; y++)
//     {
//         for (int x = 0; x < size; x++)
//         {
//             char c = 0;
//             if (grid[y][x] == 0)
//                 c = 255;
//             else
//                 c = 0;
//             img.set_pixel(x, y, c, c, c);
//         }
//     }
//     img.write("carpet.bmp");
//     std::cout << AnalysisTools::CalculateFractalDimension(grid, grid.size());
//     return 0;
// }
