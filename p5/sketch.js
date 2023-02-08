//Grid Setup
let WALKER_SIZE = 4;
let WIDTH = undefined;
let HEIGHT = undefined;
let COLS = 0;
let ROWS = 0;

let tree = [];
let dry = [];
let frostGrowth = new TupleSet();
let drySpots = new TupleSet();
let walkers = [];
//var r = 4;
let maxWalkers = 50;
let iterations = 100;
let radius = 8;
let shrink = 0.995;

let totalWalkerCount = 0;

//Drying
let R1 = 3;

function setup()
{
  WIDTH = windowWidth;
  HEIGHT = windowHeight;

  createCanvas(WIDTH, HEIGHT);
  COLS = floor(WIDTH / (WALKER_SIZE))
  ROWS = floor(HEIGHT / (WALKER_SIZE))

  tree[0] = new Walker(COLS / 2, ROWS / 2);
  // radius *= shrink;
  for (var i = 0; i < maxWalkers; i++)
  {
    walkers[i] = new Walker();
    totalWalkerCount++;
    // radius *= shrink;
  }
}

function draw()
{
  background(217, 241, 255);
  for (let i = 0; i < dry.length; i++)
  {
    dry[i].show();
  }

  for (let i = 0; i < tree.length; i++)
  {
    tree[i].show();
  }

  for (let i = 0; i < walkers.length; i++)
  {
    walkers[i].show();
  }

  for (let n = 0; n < iterations; n++)
  {
    flag = false;
    for (let i = walkers.length - 1; i >= 0; i--)
    {
      walkers[i].walk();
      if (walkers[i].checkStuck())
      {
        flag = true;
        //Drying
        walkers[i].setColor(color(1, 1, 1));
        frostGrowth.add(walkers[i].getSetForm())
        tree.push(walkers[i]);
        walkers.splice(i, 1);
      }
    }
    if (flag)
      calculateDrying();
  }

  while (walkers.length < maxWalkers && totalWalkerCount < 400)
  {
    // radius *= shrink;
    walkers.push(new Walker());
    totalWalkerCount++;
  }
}