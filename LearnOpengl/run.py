import os
import shutil
from multiprocessing import cpu_count
from typing import Union


def cp(src: Union[str], dest: Union[str]) -> None:
    if os.path.isdir(src):
        shutil.copytree(src, dest)
    elif os.path.isfile(src):
        # destination is the absolute path of new file.
        mkdir(os.path.dirname(dest))
        shutil.copy(src, dest)


def touch(path: Union[str]) -> None:
    if os.path.isdir(path):
        pass
    else:
        mkdir(os.path.dirname(path))
        with open(path, "a"):
            pass


def rm(path: str) -> None:
    if os.path.isfile(path):
        os.remove(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)


def mkdir(path: str) -> None:
    if os.path.isfile(path):
        os.remove(path)
    os.makedirs(path, exist_ok=True)


def change_name():
    directory = os.path.dirname(os.path.abspath(__file__))
    main_cpp = os.path.join(directory, "main.cpp")
    rm(main_cpp)
    num = 0
    name = ""
    for i in os.listdir():
        if i.endswith(".cpp") and "main" not in i:
            number = int(i.split(".")[0])
            if number > num:
                num = number
                name = os.path.join(directory, i)
    cp(name, main_cpp)


if __name__ == "__main__":
    change_name()
    rm("__build__")
    mkdir("__build__")
    os.chdir("__build__")
    os.system("cmake ../")
    os.system(f"make -j{cpu_count()}")
    os.system("./LearnOpenGL")
