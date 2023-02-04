import numpy as np
import random
import math
import matplotlib.pyplot as plt

GRID_SIZE = 151
frostGrid = np.ones([GRID_SIZE,GRID_SIZE], dtype=int)*2
frostGrowth = set()

originX, originY = (GRID_SIZE//2,GRID_SIZE//2)

#Simulation Variables
R = math.sqrt(2)
C = 1

frostGrid[originY][originX] = 1
frostGrowth.add((originX,originY))

#walker Variables
walkerRadius = 30
walkerDelta = 3
walkerThreashold = 0.8

running = True
random.seed()

def getFrostProb(x,y):
    hasNeighbor = (frostGrid[y-1][x-1] == 1 or frostGrid[y-1][x] == 1 or
        frostGrid[y-1][x+1] == 1 or frostGrid[y][x-1] == 1 or frostGrid[y][x+1] == 1 or
        frostGrid[y+1][x-1] == 1 or frostGrid[y+1][x] == 1 or frostGrid[y+1][x + 1] == 1)

    if hasNeighbor:
        return 1
    else:
        return 0


def calculateDrying():
    return 1        

def distanceToOriginSq(x,y):
    return (x-originX)**2 + (y-originY)**2

def walk(x,y):
    global walkerRadius
    global running
    ox = x
    oy = y
    maxRestart = 0
    restartCount = 0
    # print("HI")

    while True:
        dir = math.floor(random.uniform(0,4))
        prevX = x
        prevY = y
        # x += random.randint(-1,2)
        # y += random.randint(-1,2)
        if dir ==1:
            x += 1
        elif dir == 2:
            x -= 1
        elif dir == 3:
            y += 1
        else:
            y -= 1

        mag = distanceToOriginSq(x,y)

        # drawCur(x,y)
        
        if mag > walkerRadius**2:
            if restartCount < maxRestart:
                restartCount +=1
                x = ox
                y = oy
            else:
                break
        
        if frostGrid[y][x] != 2:
            if restartCount < maxRestart:
                restartCount +=1
                x = prevX
                y = prevY
            else:
                break

        frostProb = getFrostProb(x,y)
        # print(x,y)

        if random.random() < frostProb:
            # print("stuck")
            frostGrid[y][x] = 1
            calculateDrying()

            if mag >= (walkerThreashold*walkerRadius)**2:
                walkerRadius += walkerDelta
                if walkerRadius >= GRID_SIZE//2:
                    print("PAUSE")
                    running = False
                    walkerRadius -= walkerDelta
                else:
                   print("Radius: ", mag)
            break
        

def drawCur(x,y):
    tmp = frostGrid[y][x]
    frostGrid[y][x] = 3
    plt.imshow(frostGrid, interpolation='none')
    plt.draw()
    plt.pause(0.00001)
    plt.clf()
    frostGrid[y][x] = tmp

def draw(clear):
    plt.imshow(frostGrid, interpolation='none')
    plt.draw()
    plt.pause(0.00001)
    if clear:
        plt.clf()

#Plot
# plt.show(block=False)
itr = 0
while running:
    # print("moving")
    walkerTheta = random.random()*2*math.pi
    # print(walkerTheta)
    walkerX = math.floor(math.cos(walkerTheta)*walkerRadius) + originX
    walkerY = math.floor(math.sin(walkerTheta)*walkerRadius) + originY
    # print(walkerX,walkerY)
    walk(walkerX,walkerY)
    # if itr % 5 == 0:
    #     draw()

    # itr +=1
# draw(clear=False)
plt.imshow(frostGrid, interpolation='none')
plt.show()
    
