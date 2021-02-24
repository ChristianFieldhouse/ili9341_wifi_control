import requests
from make_raw_im import write_bin
import os
import numpy as np
from PIL import Image

url = "http://192.168.1.142/upload"

c = 0
while True:
    fn = f"image_dump/img{c}.bin"
    c += 1
    os.system("""scrot -o -z "image_dump/temp_screenshot.png" """)
    write_bin(np.array(Image.open("image_dump/temp_screenshot.png")), fn=fn, s=4)
    requests.post(url, files={'file': open(fn, 'rb')})
    #print("ok?")
