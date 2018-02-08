import model
import os


def weight0():
    mod = model.Model()
    mod.save_weight("../weights/0.pkl")
    with open("../weights/current/0", mode="w"):
        pass
    with open("../weights/best/0", mode="w"):
        pass


def get_weight_path(index):
    return "../weights/{0}.pkl".format(index)


def get_current_index():
    return int(os.listdir("../weights/current")[0])


def get_best_index():
    return int(os.listdir("../weights/best")[0])