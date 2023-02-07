class Walker
{
  constructor(x, y)
  {
    if (arguments.length == 2)
    {
      this.pos = createVector(floor(x), floor(y));
      frostGrowth.add([floor(x), floor(y)])
      this.stuck = true;
    } else
    {
      this.pos = randomPoint();
      this.stuck = false;
    }
    this.r = WALKER_SIZE / 2;
  }

  walk()
  {
    //Generate -1,0,1 for both x and y
    // var vel = p5.Vector.random2D();
    var vel = createVector(random([-1, 1, 0]), random([-1, 1, 0]));
    this.pos.add(vel);
    // console.log(vel)
    this.pos.x = constrain(this.pos.x, 0, COLS);
    this.pos.y = constrain(this.pos.y, 0, ROWS);
  }

  checkStuck(others)
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
      if (random(1) < 0.5)
      {
        this.stuck = true;
        return true;
      }
    }
    return false;
    for (var i = 0; i < others.length; i++)
    {
      let d = distSq(this.pos, others[i].pos);
      if (d < 2)
      {
        if (random(1) < 1)
          //Add sticking probability
          this.stuck = true;
        return true;
      }
    }
    return false;
  }

  setHue(hu)
  {
    this.hu = hu;
  }

  show()
  {
    noStroke();
    if (this.stuck && typeof this.hu !== 'undefined')
    {
      fill(this.hu, 255, 100, 200);
    } else
    {
      fill(360, 0, 255);
    }
    ellipse(floor(this.pos.x * WALKER_SIZE), floor(this.pos.y * WALKER_SIZE), this.r * 2, this.r * 2);
  }

  getSetForm()
  {
    return [floor(this.pos.x), floor(this.pos.y)];
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
  c1 = 1;
  c2 = 10;
  C = 0.1;

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