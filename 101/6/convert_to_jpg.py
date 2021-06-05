from PIL import Image

img = Image.open("./build/binary.ppm")
img.save("./build/binary.jpg")
img.show()