from PIL import Image
import numpy as np

def write_bin(im, x=0, y=0, s=1, fn="img.bin"):

    r_im = im[x: x+240*s:s, y:y+320*s:s, 0] >> 3
    g_im = im[x: x+240*s:s, y:y+320*s:s, 1] >> 2
    b_im = im[x: x+240*s:s, y:y+320*s:s, 2] >> 3

    b = np.zeros(2 * 320 * 240, dtype="uint8")

    b[::2] = ((r_im << 3) | (g_im >> 3)).flat
    b[1::2] = ((g_im << 5) | b_im).flat

    with open(fn, "wb") as f:
        f.write(bytes(b))

if __name__ == "__main__":
    im = np.array(Image.open(r"/home/christian/Pictures/cosmos-flower.jpg"), dtype="uint8")
    write_bin(im, 150, 150, 4, "flower.bin")
