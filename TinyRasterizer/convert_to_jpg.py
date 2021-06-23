from PIL import Image
import os


def convert_to_jpg(filename: str):
    try:
        fullname = os.path.abspath(filename)
        dirname, basename = os.path.split(fullname)
        img = Image.open(fullname)
        img.save(os.path.join(dirname, f"{basename}.jpg"))
    except Exception as e:
        print(f"Cannot convert {filename}." , e)


def walker():
    for i, _, k in os.walk(os.path.dirname(os.path.abspath(__file__))):
        for s in k:
            for formats in (".tga", "ppm"):
                if s.endswith(formats):
                    convert_to_jpg(os.path.join(i, s))


if __name__ == "__main__":
    walker()
