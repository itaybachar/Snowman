#include "Helper.h"
#include <iostream>
#include "../bmp/libbmp.h"
#include <cstdlib>

namespace Helper
{
    static int bitmapCount = 0;
    bool hasHoles(Vec2 walker, Experiment *e)
    {
        int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};
        int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};

        int flipCount = 0;
        bool prevCon, curCon;
        int **data = e->GetData();
        prevCon = curCon = (data[walker.y][walker.x] == e->WET);

        for (int i = 0; i < 8; i++)
        {
            if ((int)walker.x + dx[i] < 0 || (int)walker.x + dx[i] >= e->GetGridSize() ||
                (int)walker.y + dy[i] < 0 || (int)walker.y + dy[i] >= e->GetGridSize())
                curCon = true;
            else
                curCon = data[walker.y + dy[i]][walker.x + dx[i]] == e->WET;

            if (prevCon != curCon)
                flipCount++;
            prevCon = curCon;
        }
        return flipCount > 3;
    }

    double FrostProbability(Vec2 walker, Experiment *e)
    {
        double weightedSum = 0.0;
        int L = 9;
        int totalPointCount = (L-1)*(L-1);
        int halfCircle = L / 2;

        for (int j = walker.y - halfCircle; j <= walker.y + halfCircle; j++)
        {
            if (j < 0 || j >= e->GetGridSize())
                continue;
            for (int i = walker.x - halfCircle; i <= walker.x + halfCircle; i++)
            {
				totalPointCount++;
                if (i < 0 || i >= e->GetGridSize())
                    continue;
                if (i != walker.x || j != walker.y)
                {
                    weightedSum += 1 / (double)walker.distSqrd({i, j});
                }
            }
        }

		//std::cout << weightedSum << " "<< totalPointCount << std::endl;
		//double k = weightedSum / totalPointCount - (double)(L - 1.0) / (2.0 * L);
		double k = weightedSum / totalPointCount - (((double)totalPointCount - 2) / totalPointCount / 4.0);
        double p = e->A * k + e->B;
        p = (p < 0) ? 0.01 : (p > 1) ? 1
                                     : p;
			
        return p;
    }

    double GenerateRandomVariable()
    {
		double out = (double)rand() / RAND_MAX;
        return out;
    }

    int constrain(int val, int min, int max)
    {
        return (val < min) ? min : (val > max) ? max
                                               : val;
    }

    void GenerateBitmap(Experiment *e)
    {
        struct RGB
        {
            int r;
            int g;
            int b;
        } color;
		color = { 0,0,0 };
        BmpImg img(e->GetGridSize(), e->GetGridSize());

        const int DRY = e->DRY;

        for (int i = 0; i < e->GetGridSize(); i++)
        {
            for (int j = 0; j < e->GetGridSize(); j++)
            {
                switch (e->GetData()[i][j])
                {
                case Experiment::DRY:
                    color.r = 255;
                    color.g = 0;
                    color.b = 0;
                    break;
                case Experiment::WET:
                    color.r = 250;
                    color.g = 250;
                    color.b = 250;
                    break;
                case Experiment::FROZEN:
					color.r = 0;
					color.g = 255;
					color.b = 0;
                  //  color.r = 50;
                  //  color.g = 178;
                  //  color.b = 247;
                    break;
                }

                img.set_pixel(j, i, color.r, color.g, color.b);
            }
        }
        char str[1024];
        sprintf_s(str, "temp\\dla-%d.bmp", bitmapCount++);
        img.write(str);
    }
}