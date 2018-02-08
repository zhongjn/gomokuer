import os


def init_dirs():

    dirs = [
        "../games/backup", "../games/raw", "../games/train", "../weights/current",
        "../weights/log", "../weights/best", "../weights/eval/queue",
        "../weights/eval/result"
    ]

    for d in dirs:
        if not os.path.exists(d):
            os.mkdir(d)