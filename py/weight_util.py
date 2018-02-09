import model
import os
import config as C


def weight0():
    mod = model.Model()
    mod.save_weight(C.WEIGHTS_DIRECTORY + "/0.pkl")
    with open(C.WEIGHTS_DIRECTORY + "/current/0", mode="w"):
        pass
    with open(C.WEIGHTS_DIRECTORY + "/best/0", mode="w"):
        pass


def get_weight_path(index):
    return C.WEIGHTS_DIRECTORY + "/{0}.pkl".format(index)


def get_current_index():
    return int(os.listdir(C.WEIGHTS_DIRECTORY + "/current")[0])


def get_best_index():
    return int(os.listdir(C.WEIGHTS_DIRECTORY + "/best")[0])
