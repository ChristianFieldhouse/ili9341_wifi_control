from PIL import Image
import numpy as np

im = np.array(Image.open(r"/home/christian/Pictures/cosmos-flower.jpg"), dtype="uint8")

x = 100
y = 100
s = 4

r_im = im[x: x+240*s:s, y:y+320*s:s, 0] >> 3
g_im = im[x: x+240*s:s, y:y+320*s:s, 1] >> 2
b_im = im[x: x+240*s:s, y:y+320*s:s, 2] >> 3

b = np.zeros(2 * 320 * 240, dtype="uint8")

b[::2] = ((r_im << 3) | (g_im >> 3)).flat
b[1::2] = ((g_im << 5) | b_im).flat

with open("img.bin", "wb") as f:
    f.write(bytes(b))
