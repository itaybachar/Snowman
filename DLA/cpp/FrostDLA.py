import os, glob
import moviepy.editor as mp
from PIL import Image
from ctypes import *
from tqdm import tqdm
from halo import Halo

import math,random

def make_gif(name):
    frames = [Image.open(image) for image in glob.glob("./temp/*.png")]
    frame_one = frames[0]
    frame_one.save(f"./out_gif/{name}.gif", format="GIF", append_images=frames,
               save_all=True, duration=100, loop=0)
    clip = mp.VideoFileClip(f"./out_gif/{name}.gif")
    clip.write_videofile(f"./out_gif/{name}.mp4")

def convertToPng():
    files = glob.glob('./temp/*.bmp')

    # Converts the images:
    for f in tqdm(files, desc="Convert BMPs"):
        im = Image.open(f)
        im.convert("RGB")
        im.save(f.replace("bmp", "png"))

def delpictemp():
    # Have to write it this way to keep README.txt in pictemp 
    bmps = glob.glob("./temp/*.bmp")
    for b in tqdm(bmps[:-1], desc="Delete BMPs"):
        os.remove(b)

    pngs = glob.glob("./temp/*.png")
    for j in tqdm(pngs[:-1], desc="Delete PNGs"):
        os.remove(j)
def delpictempAll():
    # Have to write it this way to keep README.txt in pictemp 
    bmps = glob.glob("./temp/*.bmp")
    for b in tqdm(bmps, desc="Delete BMPs"):
        os.remove(b)

    pngs = glob.glob("./temp/*.png")
    for j in tqdm(pngs, desc="Delete PNGs"):
        os.remove(j)
                

def frost(humidity, A, gridSize, maxFrozenSites, snapshotInterval):
    fr = CDLL(".\\FrostGrowthDLA.dll")
    fr.SimulateFrost.argtypes = (c_double, c_double, c_int, c_int, c_int)
    fr.SimulateFrost(c_double(humidity), c_double(A), c_int(gridSize), c_int(maxFrozenSites), c_int(snapshotInterval))

def frostPro(humidity, A, gridSize, maxFrozenSites, snapshotInterval, nuclei,nucleusCount, spawnRadius):
    fr = CDLL(".\\FrostGrowthDLA.dll")
    fr.SimulateFrostPro.argtypes = (c_double, c_double, c_int, c_int, c_int,POINTER(c_int),c_int,c_int)
    fr.SimulateFrostPro(c_double(humidity), c_double(A), c_int(gridSize),
                         c_int(maxFrozenSites), c_int(snapshotInterval),
                         (c_int*(nucleusCount*2))(*nuclei),c_int(nucleusCount), c_int(spawnRadius))

def generateRandomNucleationSites(count, radius, gridSize):
    out = []

    for i in range(count):
        angle = 2*math.pi*random.random()
        out.append(int(radius*math.cos(angle)+gridSize/2))
        out.append(int(radius*math.sin(angle)+gridSize/2))

    return out

if __name__ == "__main__":
    #Clean out temp folder in case its not clean
    delpictempAll()

    humidity = 0.5
    A = 3
    gridSize = 512
    maxFrozenSites = 200000
    snapshotInterval = 750

    nucleusCount = 1
    nucleusRadius =100

    nucleationSites = generateRandomNucleationSites(nucleusCount,nucleusRadius,gridSize)
    #nucleationSites = [203,230,259,299,292,276,295,270]
    nucleationSites = [int(gridSize/2), int(gridSize/2)]
    nucleationSites = [180,256,320,100,330,250]#med humidity nucleus
   # nucleationSites = [180,256,430,10,482,500]#high humidity nucleus

    frostPro(humidity,A,gridSize,maxFrozenSites,snapshotInterval,nucleationSites,int(len(nucleationSites)/2), nucleusRadius*2)
    #frost(humidity,A,gridSize,maxFrozenSites,snapshotInterval)
    convertToPng()
    make_gif("sim_{}_{}_{}".format(humidity,A,int(len(nucleationSites)/2)))
    delpictemp()


    #
    #203 230
#259 299
#292 276
#295 270
    #
    #
