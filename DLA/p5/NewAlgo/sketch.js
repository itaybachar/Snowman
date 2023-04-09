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

let maxWalkers = 50;
let iterations = 150;
let radius = 8;
let shrink = 0.995;

let totalWalkerCount = 0;

//Spawning
let spawnRadius = 15;
let seed = undefined;

//Drying
let C_slider, c1_slider, c2_slider, restartButton, R1_slider, totalWalker_slider;
let C = 0.1;// Range of C is 0-10, 0 is 100% humidity, 10 is 0%

function setup()
{
  WIDTH = windowWidth;
  HEIGHT = windowHeight;
  WIDTH = 1000;
  HEIGHT = 1000;

  createCanvas(WIDTH, HEIGHT);
  COLS = floor(WIDTH / (WALKER_SIZE))
  ROWS = floor(HEIGHT / (WALKER_SIZE))

  seed = createVector(floor(COLS / 2), floor(ROWS / 2));

  tree[0] = new Walker(seed.x, seed.y);
  // tree[1] = new Walker(COLS / 4, ROWS / 4);

  // radius *= shrink;
  for (var i = 0; i < maxWalkers; i++)
  {
    walkers[i] = new Walker();
    totalWalkerCount++;
    // radius *= shrink;
  }

  // create sliders
  // C_slider = createSlider(0.01, 10, 0.1, 0.01);
  // C_slider.position(20, 10);
  // c1_slider = createSlider(0, 10, 1, 0.1);
  // c1_slider.position(20, 20);
  // c2_slider = createSlider(0, 100, 10, 0.1);
  // c2_slider.position(20, 30);

  // R1_slider = createSlider(Math.sqrt(2), 10, Math.sqrt(2), 0.1);
  // R1_slider.position(20, 40);
  totalWalker_slider = createSlider(100, ROWS * COLS, 10000, 1);
  totalWalker_slider.position(20, 50);

  // // C_slider.style('width', '50px');
  // c1_slider.style('width', '50px');
  // c2_slider.style('width', '50px');
  // R1_slider.style('width', '50px');
  totalWalker_slider.style('width', '50px');

  // textSize(5);

  // //Create button
  // restartButton = createButton('Restart');
  // restartButton.position(20, 70);
  // restartButton.mousePressed(restartSimulation);
  // restartButton.size(40, 15);
  // restartButton.style("font-size", "7px");

}

function draw()
{
  background(217, 241, 255);
  // fill(0);
  // noStroke();
  // // text('C: ' + C_slider.value(), C_slider.x * 1.5 + C_slider.width, C_slider.y + 10);
  // text('C: ' + C, 20 * 1.5, 10 + 10);

  // text('C1: ' + c1_slider.value(), c1_slider.x * 1.5 + c1_slider.width, c1_slider.y + 10);
  // text('C2: ' + c2_slider.value(), c2_slider.x * 1.5 + c2_slider.width, c2_slider.y + 10);
  // text('R1: ' + R1_slider.value(), R1_slider.x * 1.5 + R1_slider.width, R1_slider.y + 10);
  // text('Total Walkers: ' + totalWalker_slider.value(), totalWalker_slider.x * 1.5 + totalWalker_slider.width, totalWalker_slider.y + 10);

  noFill();
  stroke(color(255, 0, 0));
  circle(floor(seed.x * WALKER_SIZE), floor(seed.y * WALKER_SIZE), spawnRadius * WALKER_SIZE);

  for (let i = 0; i < dry.length; i++)
  {
    dry[i].show();
  }

  for (let i = 0; i < tree.length; i++)
  {
    tree[i].show();
  }

  for (let n = 0; n < iterations; n++)
  {
    flag = false;
    for (let i = walkers.length - 1; i >= 0; i--)
    {
      walkers[i].walk();
      let status = walkers[i].checkStuck()
      if (status > 0)
      {
        if (status == 1)
        {
          flag = true;
          walkers[i].setColor(color(1, 1, 1));
          frostGrowth.add(walkers[i].getSetForm())
          tree.push(walkers[i]);

        }
        //Drying
        walkers.splice(i, 1);
      }
    }
    // if (flag)
    //   calculateDrying();
  }

  while (walkers.length < maxWalkers && totalWalkerCount < totalWalker_slider.value())
  {
    // radius *= shrink;
    walkers.push(new Walker());
    totalWalkerCount++;
  }
}

function restartSimulation()
{
  tree = [];
  dry = [];
  frostGrowth = new TupleSet();
  drySpots = new TupleSet();
  walkers = [];
  totalWalkerCount = 0;

  tree[0] = new Walker(COLS / 2, ROWS / 2);
  for (var i = 0; i < maxWalkers; i++)
  {
    walkers[i] = new Walker();
    totalWalkerCount++;
  }
}