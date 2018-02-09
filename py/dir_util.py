import os
import config as C


def init_dirs():

    dirs = [
        "../games/backup", "../games/raw", "../games/train",
        C.WEIGHTS_DIRECTORY + "/current", C.WEIGHTS_DIRECTORY + "/log",
        C.WEIGHTS_DIRECTORY + "/best", C.WEIGHTS_DIRECTORY + "/eval/queue",
        C.WEIGHTS_DIRECTORY + "/eval/result"
    ]

    for d in dirs:
        if not os.path.exists(d):
            os.makedirs(d, exist_ok=True)