//Grid Setup
let WALKER_SIZE = 8;
let WIDTH = undefined;
let HEIGHT = undefined;
let COLS = 0;
let ROWS = 0;

let tree = [];
let frostGrowth = new TupleSet();
let drySpots = new TupleSet();
let walkers = [];
//var r = 4;
let maxWalkers = 50;
let iterations = 1000;
let radius = 8;
let hu = 0;
let shrink = 0.995;

let totalWalkerCount = 0;

//Drying
let R1 = 1.4142135623730951

function setup()
{
  WIDTH = windowWidth;
  HEIGHT = windowHeight;

  createCanvas(WIDTH, HEIGHT);
  colorMode(HSB);
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
  background(0);

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
    for (let i = walkers.length - 1; i >= 0; i--)
    {
      walkers[i].walk();
      if (walkers[i].checkStuck(tree))
      {
        //Drying
        calculateDrying();
        walkers[i].setHue(hu % 360);
        // hu += 2;
        frostGrowth.add(walkers[i].getSetForm())
        tree.push(walkers[i]);
        walkers.splice(i, 1);
      }
    }
  }

  // var r = walkers[walkers.length - 1].r;
  while (walkers.length < maxWalkers && totalWalkerCount < 800)
  {
    // radius *= shrink;
    walkers.push(new Walker());
    totalWalkerCount++;
  }
}