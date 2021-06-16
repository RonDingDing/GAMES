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
    rm('__build__')
    rm('.vscode')
    rm('.cache')
    rm('compile_commands.json')

