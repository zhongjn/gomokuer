import os
import numpy as np
import time
import utilities
import uuid


def run():
    GAMES_PER_FILE = 2500

    games = 0
    files = os.listdir("../games/raw")

    # clean damaged files
    print("cleaning broken files...")
    for f in files:
        t = f.split(".")
        if len(t) == 3 and t[2] == "npy":
            filename = t[0] + ".npz"
            try:
                os.remove("../games/raw/" + f)
                os.remove("../games/raw/" + filename)
            except FileNotFoundError as fe:
                print(fe)

    files = os.listdir("../games/raw")

    print("backing up...")
    utilities.make_zip("../games/raw", "../games/backup/{0}.zip".format(
        uuid.uuid1()))

    count = len(files)
    for i in range(count):
        f = files[i]

        if (i + 1) % 100 == 0:
            print("merging {0} of {1}".format(i + 1, count))

        if games == 0:
            observation = []
            prob = []
            result = []

        with np.load("../games/raw/" + f) as data:
            observation.append(data["observation"])
            prob.append(data["prob"])
            result.append(data["result"])

            games += 1
            if games >= GAMES_PER_FILE or i == len(files) - 1:
                observation = np.concatenate(observation)
                prob = np.concatenate(prob)
                result = np.concatenate(result)

                np.savez(
                    "../games/train/{time}-{count}.npz".format(
                        time=time.strftime("%Y%m%d%H%M%S"), count=games),
                    observation=observation,
                    prob=prob,
                    result=result)
                games = 0



    # shutil.rmtree("../games/raw")
    time.sleep(1)
    # os.mkdir("../games/raw")
    print("cleaning...")
    for f in files:
        try:
            os.remove("../games/raw/" + f)
        except Exception as e:
            print(e)


if __name__ == "__main__":
    run()