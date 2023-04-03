class Walker
{
  constructor(x, y, dry = false)
  {
    if (arguments.length >= 2)
    {
      this.pos = createVector(floor(x), floor(y));
      if (!dry)
        frostGrowth.add([floor(x), floor(y)])
      this.stuck = true;
    } else
    {
      this.pos = randomPoint();
      this.stuck = false;
    }
    this.dry = dry;
    this.r = WALKER_SIZE / 2;
    if (dry)
      this.r = constrain(this.r - 1, 0.5, this.r);
  }

  walk()
  {
    //Generate -1,0,1 for both x and y
    // var vel = p5.Vector.random2D();
    var vel = createVector(random([-1, 1, 0]), random([-1, 1, 0]));
    this.pos.add(vel);
    if (frostGrowth.has(this.getSetForm()) || drySpots.has(this.getSetForm()))
      this.pos.sub(vel)
    // console.log(vel)
    //Constrain To Circle
    //Add if statement to see if spawnRadius could grow
    this.pos.x = constrain(this.pos.x, seed.x - spawnRadius, seed.x + spawnRadius)
    this.pos.y = constrain(this.pos.y, seed.y - spawnRadius, seed.y + spawnRadius)

    //Constrain To Grid:
    this.pos.x = constrain(this.pos.x, 0, COLS);
    this.pos.y = constrain(this.pos.y, 0, ROWS);
  }

  checkStuck()
  {
    if (
      frostGrowth.has([this.pos.x, this.pos.y - 1]) ||
      frostGrowth.has([this.pos.x - 1, this.pos.y]) ||
      frostGrowth.has([this.pos.x + 1, this.pos.y]) ||
      frostGrowth.has([this.pos.x, this.pos.y + 1]))
    {
      //Make sure no holes are formed
      if (!hasHoles(this.pos))
      {
        //Calculate freezing probability
        let prob = getFrostProbability(this.pos.x, this.pos.y);
        if (random(1) < prob)
        {
          //Walker Froze
          this.stuck = true;

          if (distSq(this.pos, seed) >= 0.8 * spawnRadius * spawnRadius)
          {
            spawnRadius = constrain(spawnRadius + 2, spawnRadius, Math.min(seed.x, seed.y));
          }
          return 1;
        }
        else
        {
          //If walker did not freeze, 
          //First allow the seed to grow a little
          if (tree.length > 100)
          {
            //Probability of Drying
            //If we satisfy the probability, we are dry. else we walk.
            if (random(1) < (1 - humidity) * prob)
            {
              this.dry = true;
              this.stuck = true;
              this.setColor(color(0, 0, 80));
              dry.push(this)
              drySpots.add([this.pos.x, this.pos.y]);
              return 2;
            }
          }
        }
      }
    }
    return 0;
  }

  setColor(c)
  {
    this.c = c;
  }

  show()
  {
    noStroke();
    if (this.stuck && typeof this.c !== 'undefined')
    {
      if (!this.dry)//Dry
      {
        stroke(0);
        strokeWeight(0.5);
        this.c = color(87, 191, 235);
      }
      fill(this.c);
    } else
    {//Seed
      if (this.stuck)
        fill(30, 30, 30);
      else
        fill(255, 255, 255);
    }
    ellipse(floor(this.pos.x * WALKER_SIZE), floor(this.pos.y * WALKER_SIZE), this.r * 2, this.r * 2);
  }

  getSetForm()
  {
    return [this.pos.x, this.pos.y];
  }

}

function randomPoint()
{
  //Spawn in a circle
  angle = random(Math.PI * 2);
  let x = floor(Math.cos(angle) * spawnRadius + seed.x);
  let y = floor(Math.sin(angle) * spawnRadius + seed.y);
  return createVector(x, y);
  //Spawn on Edges
  /*
  var i = floor(random(4));

  if (i === 0)
  {
    let x = floor(random(COLS));
    return createVector(x, 0);
  } else if (i === 1)
  {
    let x = floor(random(COLS));
    return createVector(x, ROWS - 1);
  } else if (i === 2)
  {
    let y = floor(random(ROWS));
    return createVector(0, y);
  } else
  {
    let y = floor(random(ROWS));
    return createVector(COLS - 1, y);
  }
  */
}

function distSq(a, b)
{
  var dx = b.x - a.x;
  var dy = b.y - a.y;
  return dx * dx + dy * dy;
}

function calculateDrying()
{
  R1 = R1_slider.value();
  c1 = c1_slider.value();
  c2 = c2_slider.value();
  // C = C_slider.value();

  R1Sqrd = R1 * R1;
  for (let y = 0; y < ROWS; y++)
  {
    for (let x = 0; x < COLS; x++)
    {
      if (!frostGrowth.has([x, y]) && !drySpots.has([x, y]))
      {
        //Get all points around x,y with radius R
        //So we create a square, then see if the points is outside a circle
        let totalProbability = 0;
        for (let j = y - ceil(R1); j <= y + ceil(R1); j++)
        {
          for (let k = x - ceil(R1); k <= x + ceil(R1); k++)
          {
            //Check in circle
            let distSqr = distSq(createVector(x, y), createVector(k, j));
            if (distSqr <= R1Sqrd)
            {
              if (frostGrowth.has([k, j]))
                totalProbability += C / (c1 * distSqr + c2);
            }
          }
        }

        if (totalProbability > 0)
        {
          //Dry
          if (random(1) < totalProbability)
          {
            let w = new Walker(x, y, true)
            w.setColor(color(2, 2, 2));
            dry.push(w)
            drySpots.add([x, y]);
          }
        }
      }
    }
  }
}

function hasHoles(pos)
{
  //go around the point(3 by 3) and find all the "flips"
  deltax = [0, -1, -1, -1, 0, 1, 1, 1]
  deltay = [1, 1, 0, -1, -1, -1, 0, 1]

  flipCount = 0;
  prevCon = curCon = frostGrowth.has([pos.x + 1, pos.y + 1]);
  for (let index = 0; index < 8; index++)
  {
    curCon = frostGrowth.has([pos.x + deltax[index], pos.y + deltay[index]]);
    if (prevCon != curCon)
    {
      flipCount++;
    }
    prevCon = curCon;
  }
  //Get points around  potential stick in a clockwise manner
  return flipCount > 3;
}