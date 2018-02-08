import numpy as np
import os
import re
import config as C


def transform(array, action, inverse=False):
    if action == 0:
        return array
    elif action == 1:
        return np.transpose(array, [0, 2, 1])
    elif action == 2:
        return np.flip(array, 1)
    elif action == 3:
        if inverse:
            return np.rot90(array, -1, (1, 2))
        else:
            return np.rot90(array, 1, (1, 2))
    elif action == 4:
        return np.rot90(array, 2, (1, 2))
    elif action == 5:
        if inverse:
            return np.rot90(np.transpose(array, [0, 2, 1]), 2, (1, 2))
        else:
            return np.transpose(np.rot90(array, 2, (1, 2)), [0, 2, 1])
    elif action == 6:
        return np.flip(array, 2)
    elif action == 7:
        if inverse:
            return np.rot90(array, 1, (1, 2))
        else:
            return np.rot90(array, -1, (1, 2))


def load():
    directory = "../games/train/"
    count_sum = 0

    observation = []
    prob = []
    result = []

    files = os.listdir(directory)
    files.reverse()

    for f in files:
        sp = re.split("-|\.", f)
        count_sum += int(sp[1])
        data = np.load(directory + f)

        observation.append(data["observation"])
        prob.append(data["prob"])
        result.append(data["result"])

        if (count_sum >= C.RECENT_COUNT):
            break

    return np.concatenate(observation), np.concatenate(prob), np.concatenate(
        result)


def augment(observation, prob, result):
    count = np.size(observation, 0)
    action = np.random.randint(0, 8, count, int)
    for i in range(count):
        observation[i] = transform(observation[i], action[i], False)

        p = np.resize(prob[i], [1, C.BOARD_SIZE, C.BOARD_SIZE])
        prob[i] = np.resize(
            transform(p, action[i], False), [C.BOARD_SIZE * C.BOARD_SIZE])
    return observation, prob, result


def shuffle(observation, prob, result):
    count = np.size(observation, 0)
    shuffle = np.random.RandomState(seed=10).permutation(count)
    return observation[shuffle], prob[shuffle], result[shuffle]
