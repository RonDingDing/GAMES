from PIL import Image


def convert_to_jpg(filename: str):
    img = Image.open(f"./__build__/{filename}.ppm")
    img.save(f"./__build__/{filename}.jpg")
    img.show()


if __name__ == "__main__":
    convert_to_jpg("output")
