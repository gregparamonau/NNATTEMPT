#creates two files: training, and testing dataset
#80% training, 20% testing
from shapely.geometry import Polygon, Point
from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random

verts_6 = [(-1, 1.5), (-0.5, 1.25), (-1.25, -0.25), (-1.75, 0)]
verts_7 = [(0.25, 1.5), (1.75, 1.5), (1, -1.4), (0.5, -1.4), (1.1, 1), (0.25, 1)]

six = Polygon(verts_6)
seven = Polygon(verts_7)

IMG_W, IMG_H = 800, 500
img = Image.new('L', (IMG_W, IMG_H), color=255)
draw = ImageDraw.Draw(img)
font = ImageFont.truetype("/System/Library/Fonts/Supplemental/Futura.ttc", size=300)
draw.text((40, 25), "Greg", font=font, fill=0)
pixels = list(img.getdata())
pixels = [pixels[i*IMG_W:(i+1)*IMG_W] for i in range(IMG_H)]

def inside_circle(input):
    return (input[0] + 1) ** 2 + (input[1] + 0.5) ** 2 <= 0.8

def create_input (input):
    return input

def create_output(input):
    x, y = input
    px = int((x + 4) / 8 * IMG_W)
    py = int((1 - (y + 2.5) / 5) * IMG_H)
    px = max(0, min(IMG_W - 1, px))
    py = max(0, min(IMG_H - 1, py))
    frac = 1 if pixels[py][px] < 128 else 0
    return [frac, 1 - frac]

'''def create_output(input):
    (x, y) = input
    p = Point(x, y)
    frac = 1 if inside_circle(input) or p.within(six) or p.within(seven) else 0
    return [frac, 1 - frac]
    #frac = 1 if (x **2 + y **2 - 1) ** 3 - x **2 * y ** 3 <= 0 else 0
    #return [frac, 1 - frac]'''

if __name__ == "__main__":
    training_file = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/training.txt"
    testing_file = "/Users/gregoryparamonau/Desktop/VSCode/NNATTEMPT/testing.txt"
    total_data = 175000
    training_frac = 0.8

    n_in = len(create_input((0, 0)))
    n_out = len(create_output(create_input((0, 0))))
    sx = 8
    sy = 5

    #generate points first
    n = int(round(total_data ** 0.5))
    points = []

    # setup at module level
    #img = img.filter(ImageFilter.MaxFilter(size=21))
    #img.show()

    for i in range(n):
        for j in range(n):
            points.append((sx * (i - n / 2) / n, sy * (j - n / 2) / n))
    
    random.shuffle(points)

    split_index = int(round(total_data * training_frac))
    training_points = points[:split_index]
    testing_points = points[split_index:]

    with open(training_file, 'w') as f:
        f.write(str(len(training_points)) + " " + str(n_in) + " " + str(n_out) + "\n")
        for i in training_points:
            f.write("A " + " ".join(map(str, i)) + " " + " ".join(map (str, create_output(i))) + "\n")
    f.close()

    with open(testing_file, 'w') as f:
        f.write(str(len(testing_points)) + " " + str(n_in) + " " + str(n_out) + "\n")

        for i in testing_points:
            f.write("A " + " ".join(map(str, i)) + " " + " ".join(map (str, create_output(i))) + "\n")
    f.close()