A = 2;
B = 0.5;
L = 9;
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

    return 1;
    console.log(1 - (C / 10));
    return 1 - (C / 10);
}

function updateHumidity()
{
    // C += 0.01;
    // C_slider.value = 0.01;
}

