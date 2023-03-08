import os, glob
from PIL import Image
from ctypes import *
from tqdm import tqdm
from halo import Halo

@Halo(text="Making GIF")
def make_gif(name, iters):
    frames = [Image.open(image) for image in glob.glob("./Snowman/c_garbo/pictemp/*.png")]
    frame_one = frames[0]
    frame_one.save(f"./Snowman/c_garbo/{name}.gif", format="GIF", append_images=frames,
               save_all=True, duration=iters*1, loop=0)

def convertToPng():
    files = glob.glob('./Snowman/c_garbo/pictemp/*.bmp')

    # Converts the images:
    for f in tqdm(files, desc="Convert BMPs"):
        im = Image.open(f)
        im.convert("RGB")
        im.save(f.replace("bmp", "png"))

def delpictemp():
    # Have to write it this way to keep README.txt in pictemp 
    bmps = glob.glob("./Snowman/c_garbo/pictemp/*.bmp")
    for b in tqdm(bmps, desc="Delete BMPs"):
        os.remove(b)

    jpegs = glob.glob("./Snowman/c_garbo/pictemp/*.png")
    for j in tqdm(jpegs, desc="Delete PNGs"):
        os.remove(j)

def frost(temp, humidity, len, iters, pwid):
    fr = CDLL("./Snowman/c_garbo/frosting.so")
    fr.frost.argtypes = (c_int, c_int, c_int, c_int, c_int)
    fr.frost(c_int(temp), c_int(humidity), c_int(len), c_int(iters), c_int(pwid))

if __name__ == "__main__":
    iters = 80
    frost(-15, 80, 400, iters, 2)
    convertToPng()
    make_gif("frAni", iters)
    delpictemp()
