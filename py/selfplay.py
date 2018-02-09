import config as C
import os
import weight_util as util
import time
import merge


def run():
    for i in range(C.SELFPLAY_PROCESS_COUNT):
        os.system(
            "start ../gomokuer.exe selfplay -w {wdir}/{w}.pkl -r {r}".format(
                wdir=C.WEIGHTS_DIRECTORY,
                w=util.get_best_index(),
                r=(C.SELFPLAY_TARGET_ROUNDS / C.SELFPLAY_PROCESS_COUNT + 100)))

    while True:
        games = len(os.listdir("../games/raw"))
        if games >= C.SELFPLAY_TARGET_ROUNDS:
            os.system("taskkill /im gomokuer.exe /f")
            print("merging...")
            merge.run()
            break
        else:
            print("round {0} of {1}".format(games, C.SELFPLAY_TARGET_ROUNDS))
        time.sleep(5)
    print("complete")


if __name__ == "__main__":
    run()