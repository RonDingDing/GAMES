from PIL import Image

img = Image.open("./__build__/binary.ppm")
img.save("./__build__/binary.jpg")
img.show()