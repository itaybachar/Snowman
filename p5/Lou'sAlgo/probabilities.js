A = 10;
B = 0.5;
L = 3;

//High humidity - Basically grows in a circle
//Low Humidity - Drying, eventual stopping of growth
//Mid Humidity - Fractal

function getFrostProbability(x, y)
{
    Ni = 0;
    let halfCircle = floor(L / 2);
    for (let i = y - halfCircle; i <= y + halfCircle; i++)
    {
        for (let j = x - halfCircle; j <= x + halfCircle; j++)
        {
            if (frostGrowth.has([i, j]))
                Ni++;
        }
    }

    minVal = .01;
    p = A * (Ni / L / L - (L - 1) / 2 / L) + B;
    // console.log("Prob: " + p)
    if (p < minVal)
    {
        p = minVal;
    }
    return p;
}

function updateHumidity()
{
    // C += 0.01;
    // C_slider.value = 0.01;
}

