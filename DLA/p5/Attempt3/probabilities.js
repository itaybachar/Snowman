m_c1 = 0.4;
m_c2 = 0.9;
m_b1 = 0.3;
m_C = 10;
m_C2 = 0.3;

let A = 1;
let B = 0.3;
let humidity = 0.35;//0-1(0 means no humidity, 1 means most)

//High humidity - Basically grows in a circle
//Low Humidity - Drying, eventual stopping of growth
//Mid Humidity - Fractal

function getFrostProbability(x, y)
{
    Ni = 0;
    weightedSum = 0;
    L = 5;
    totalPointCount = (L - 1) * (L - 1);
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
    k = weightedSum / totalPointCount - ((totalPointCount - 2) / totalPointCount / 4);
    // k = (Ni / L / L - (L - 1) / 2 / L);

    p = A * k + B;

    p = (p < 0) ? 0.01 : (p > 1) ? 1 : p;
    console.log("Prob: " + p)
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