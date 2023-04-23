import os, glob
import moviepy.editor as mp
import matplotlib.pyplot as plt
from PIL import Image
from ctypes import *
from tqdm import tqdm

def make_gif(name, iters):
    frames = [Image.open(image) for image in glob.glob("./Snowman/c_garbo/pictemp/*.png")]
    frame_one = frames[0]
    frame_one.save(f"./Snowman/c_garbo/{name}.gif", format="GIF", append_images=frames,
               save_all=True, duration=100, loop=0)
    clip = mp.VideoFileClip(f"./Snowman/c_garbo/{name}.gif")
    clip.write_videofile(f"./Snowman/c_garbo/{name}.mp4")

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

def frost(temp, humidity, len, iters, pwid, bias, reduxind, reduxPara):
    fr = CDLL("./Snowman/c_garbo/frosting.so")
    fr.frost.argtypes = (c_int, c_int, c_int, c_int, c_int, c_double, c_int, c_double)
    fr.frost.restype = POINTER(c_double)
    return fr.frost(c_int(temp), c_int(humidity), c_int(len), c_int(iters), c_int(pwid), c_double(bias), c_int(reduxind), c_double(reduxPara))

if __name__ == "__main__":
    temp = -15 # Celsius
    humidity = 52 # humidity out of 255
    len = 200 # Creates len by len square array
    iters = 220 # numebr of iterations to do
    pwid = 2 # pixel width, makes teh image have more pixels so its easier to see
    bias = 0.5 # bias for running the random function
    reduxFunc = 0 # 0 -> No reduction, 1 -> 1% per 100 iterations (linear), 2 -> reduxPara% per 100 iterations (linear) 
    reduxPara = 0.3
    
    # The C code returns a pointer to an array of the box dimensions for for every single iteration
    # for the calcultion including both frozen and dry node (nonwetDim) and just frozen nodes (frozDim)
    boxDimensions = POINTER(c_int) 
    boxDimensions = frost(temp, humidity, len, iters, pwid, bias, reduxFunc, reduxPara)
    nonwetDim = boxDimensions[:iters]
    frozDim = boxDimensions[iters:2*iters]

    convertToPng()
    make_gif("frAni", iters)
    delpictemp()

    plt.plot(range(iters), nonwetDim, color='black', label='Nonwet Box Dimension')
    plt.plot(range(iters), frozDim, color='blue', label='Frozen Box Dimension')
    plt.xlabel('Current Iteration')
    plt.ylabel('Box Dimension')
    plt.legend()
    plt.show()