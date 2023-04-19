import os, glob
import moviepy.editor as mp
from PIL import Image
from ctypes import *
from tqdm import tqdm
from halo import Halo

def make_gif(name):
    frames = [Image.open(image) for image in glob.glob("./temp/*.png")]
    frame_one = frames[0]
    frame_one.save(f"./out_gif/{name}.gif", format="GIF", append_images=frames,
               save_all=True, duration=500, loop=0)
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
    for b in tqdm(bmps, desc="Delete BMPs"):
        os.remove(b)

    jpegs = glob.glob("./temp/*.png")
    for j in tqdm(jpegs, desc="Delete PNGs"):
        os.remove(j)

def frost(humidity, A, gridSize, maxFrozenSites, snapshotInterval):
    fr = CDLL(".\\FrostGrowthDLA.dll")
    fr.SimulateFrost.argtypes = (c_double, c_double, c_int, c_int, c_int)
    fr.SimulateFrost(c_double(humidity), c_double(A), c_int(gridSize), c_int(maxFrozenSites), c_int(snapshotInterval))

def frostPro(humidity, A, gridSize, maxFrozenSites, snapshotInterval, nuclei,nucleusCount):
    fr = CDLL(".\\FrostGrowthDLA.dll")
    fr.SimulateFrostPro.argtypes = (c_double, c_double, c_int, c_int, c_int,POINTER(c_int),c_int)
    fr.SimulateFrostPro(c_double(humidity), c_double(A), c_int(gridSize),
                         c_int(maxFrozenSites), c_int(snapshotInterval),
                         (c_int*(nucleusCount*2))(*nuclei),c_int(nucleusCount))

if __name__ == "__main__":
    humidity = 0.9
    A = 3
    gridSize = 500
    maxFrozenSites = 75000
    snapshotInterval = 750
    nucleationSites = [100,150,250,250,350,350]
    frostPro(humidity,A,gridSize,maxFrozenSites,snapshotInterval,nucleationSites,int(len(nucleationSites)/2))
    #frost(humidity,A,gridSize,maxFrozenSites,snapshotInterval)
    convertToPng()
    make_gif("sim_{}_{}".format(humidity,A))
    delpictemp()
