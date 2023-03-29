m_c1 = 0.4;
m_c2 = 0.9;
m_b1 = 0.3;
m_C = 10;
m_C2 = 0.3;

let A = 10;
let B = 0.5;

//High humidity - Basically grows in a circle
//Low Humidity - Drying, eventual stopping of growth
//Mid Humidity - Fractal

function getFrostProbability(x, y)
{
    Ni = 0;
    totalPointCount = 0;
    weightedSum = 0;
    L = 9;
    let halfCircle = floor(L / 2);
    for (let i = y - halfCircle; i <= y + halfCircle; i++)
    {
        for (let j = x - halfCircle; j <= x + halfCircle; j++)
        {
            if (i != y && j != x)
            {
                totalPointCount++;
                if (frostGrowth.has([i, j]))
                {
                    Ni++;
                    let distSqr = distSq(createVector(x, y), createVector(j, i));

                    weightedSum += 1 / distSqr;
                }
            }
        }
    }
    // console.log(weightedSum);
    minVal = .01;
    k = weightedSum / totalPointCount - ((totalPointCount - 2) / totalPointCount / 8);
    // k = (Ni / L / L - (L - 1) / 2 / L);

    p = A * k + B;
    // console.log(k)
    // console.log("Prob: " + p)
    p = (p < 0) ? 0.01 : (p > 1) ? 1 : p;
    // console.log(p)
    return p;
}

function gaussianRandom(mean = 0, stdev = 1)
{
    let u = 1 - random(); //Converting [0,1) to (0,1)
    let v = random();
    let z = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
    // Transform to the desired mean and standard deviation:
    return z * stdev + mean;
}