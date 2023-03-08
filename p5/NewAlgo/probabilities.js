m_c1 = 0.05;
m_c2 = 0.1;
m_b1 = 0.4;
m_C = 10;
m_C2 = 0.3;//0.41;

//High humidity - Basically grows in a circle
//Low Humidity - Drying, eventual stopping of growth
//Mid Humidity - Fractal

function getFrostProbability(x, y)
{
    waterConsumption = 0;
    //Check Diagonals:
    diagCount = 0;
    adjCount = 0;
    /* if (frostGrowth.has([x - 1, y - 1]))
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
     */

    Ni = 0;
    L = 9
    total = 0;
    let halfCircle = floor(L / 2);
    for (let i = y - halfCircle; i <= y + halfCircle; i++)
    {
        for (let j = x - halfCircle; j <= x + halfCircle; j++)
        {
            if (frostGrowth.has([i, j]))
                Ni++;
            total++;
        }
    }
    // tension = 1 / constrain(10 * (Ni / L / L - (L - 1) / 2 / L) + 0.2, 0.1, 100);
    /*waterConsumption = diagCount * (m_C * m_c2 + m_b1) + adjCount * (m_C * m_c1 + m_b1);
    waterConsumption = waterConsumption * (m_C2);*/
    waterConsumption = (Ni) * (m_C) + m_b1;
    // console.log(waterConsumption);

    waterConsumption = map(waterConsumption, 0, total * (m_C) + m_b1, 0, 1);
    console.log(waterConsumption);

    return 1 - waterConsumption;
}

function updateHumidity()
{
    // C += 0.01;
    // C_slider.value = 0.01;
}

function gaussianRandom(mean = 0, stdev = 1)
{
    let u = 1 - random(); //Converting [0,1) to (0,1)
    let v = random();
    let z = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
    // Transform to the desired mean and standard deviation:
    return z * stdev + mean;
}