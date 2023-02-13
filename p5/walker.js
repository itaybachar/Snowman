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
      this.r = constrain(this.r - 1, 1, this.r);
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
    this.pos.x = constrain(this.pos.x, 0, COLS);
    this.pos.y = constrain(this.pos.y, 0, ROWS);
  }

  checkStuck()
  {
    if (frostGrowth.has([this.pos.x - 1, this.pos.y - 1]) ||
      frostGrowth.has([this.pos.x, this.pos.y - 1]) ||
      frostGrowth.has([this.pos.x + 1, this.pos.y - 1]) ||
      frostGrowth.has([this.pos.x - 1, this.pos.y]) ||
      frostGrowth.has([this.pos.x + 1, this.pos.y]) ||
      frostGrowth.has([this.pos.x - 1, this.pos.y + 1]) ||
      frostGrowth.has([this.pos.x, this.pos.y + 1]) ||
      frostGrowth.has([this.pos.x + 1, this.pos.y + 1]))
    {
      //Add sticking probability
      if (random(1) < getFrostProbability(this.pos.x, this.pos.y))
      {
        this.stuck = true;
        //Update humidity
        // updateHumidity();
        return true;
      }
      else
        return false;
    }
    return false;
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
      if (this.dry)//Dry
        this.c = color(43, 0, 122);
      else//Frozen
      {
        stroke(0);
        strokeWeight(0.5);
        this.c = color(107, 211, 255);
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
/*
def calculateDrying():
    global frostGrid
    c1 = 1
    c2 = 10
    for y in range(1,GRID_SIZE-1):
        for x in range(1,GRID_SIZE-1):
            delta = 0
            if(frostGrid[y][x] == 2):
                if((x+1,y) in frostGrowth) or ((x-1,y) in frostGrowth) or ((x,y+1) in frostGrowth) or ((x,y-1) in frostGrowth):
                    delta = 1
                if ((x+1,y-1) in frostGrowth) or (x-1,y-1) in frostGrowth or (x-1,y+1) in frostGrowth or (x,y+1) in frostGrowth:
                    delta = R
            prob = 0
            if delta != 0:
                prob =  C/(c1*delta+c2)
            if(prob > random.random()):
                frostGrid[y][x] = 0
*/