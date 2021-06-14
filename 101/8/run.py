import os
import shutil

def rm(path: str) -> None:
    if os.path.isfile(path):
        os.remove(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)


def mkdir(path: str) -> None:
    if os.path.isfile(path):
        os.remove(path)
    os.makedirs(path, exist_ok=True)


if __name__ == '__main__':
    rm('build')
    mkdir('build')
    os.chdir('build')
    os.system('cmake ../')
    os.system('make')

