import os, glob
from PIL import Image
from ctypes import *

def make_gif(name, iters):
    frames = [Image.open(image) for image in glob.glob("./Snowman/c_garbo/pictemp/*.jpg")]
    frame_one = frames[0]
    frame_one.save(f"./Snowman/c_garbo/{name}.gif", format="GIF", append_images=frames,
               save_all=True, duration=iters*40, loop=0)

def convertToJpg():
    files = glob.glob('./Snowman/c_garbo/pictemp/*.bmp')

    # Converts the images:
    for f in files:
        im = Image.open(f)
        im.convert("RGB")
        im.save(f.replace("bmp", "jpg"))

def delpictemp():
    # Have to write it this way to keep README.txt in pictemp 
    bmps = glob.glob("./Snowman/c_garbo/pictemp/*.bmp")
    for b in bmps:
        os.remove(b)

    jpegs = glob.glob("./Snowman/c_garbo/pictemp/*.jpg")
    for j in jpegs:
        os.remove(j)

def frost(temp, humidity, len, iters):
    fr = CDLL("./Snowman/c_garbo/frosting.so")
    fr.frost.argtypes = (c_int, c_int, c_int, c_int)
    fr.frost(c_int(temp), c_int(humidity), c_int(len), c_int(iters))

if __name__ == "__main__":
    iters = 25
    frost(-10, 120, 1024, iters)
    convertToJpg()
    make_gif("frAni", iters)
    delpictemp()
