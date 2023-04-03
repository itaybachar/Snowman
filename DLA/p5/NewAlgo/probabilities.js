m_c1 = 0.4;
m_c2 = 0.9;
m_b1 = 0.3;
m_C = 10;
m_C2 = 0.3;

//High humidity - Basically grows in a circle
//Low Humidity - Drying, eventual stopping of growth
//Mid Humidity - Fractal

function getFrostProbability(x, y)
{
    waterConsumption = 0;
    //Check Diagonals:
    diagCount = 0;
    adjCount = 0;
    if (frostGrowth.has([x - 1, y - 1]))
        diagCount++;
    if (frostGrowth.has([x + 1, y - 1]))
        diagCount++;
    if (frostGrowth.has([x - 1, y + 1]))
        diagCount++;
    if (frostGrowth.has([x + 1, y + 1]))
        diagCount++;
    if (frostGrowth.has([x, y - 1]))
        adjCount++;
    if (frostGrowth.has([x - 1, y]))
        adjCount++;
    if (frostGrowth.has([x + 1, y]))
        adjCount++;
    if (frostGrowth.has([x, y + 1]))
        adjCount++;

    waterConsumption = diagCount * (m_C * m_c2 + m_b1) + adjCount * (m_C * m_c1 + m_b1);
    waterConsumption = waterConsumption / (4 * (m_C * m_c2 + m_b1) + 4 * (m_C * m_c2 + m_b1));

    return waterConsumption;
}

function gaussianRandom(mean = 0, stdev = 1)
{
    let u = 1 - random(); //Converting [0,1) to (0,1)
    let v = random();
    let z = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
    // Transform to the desired mean and standard deviation:
    return z * stdev + mean;
}