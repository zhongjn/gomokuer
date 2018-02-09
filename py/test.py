import numpy as np
import model
obvs = np.zeros([1, 2, 15, 15], float)
# obvs[0, 0, 13, 7] = 1
# obvs[0, 1, 0, 0:4] = 1
# obvs[0, 1, 6, 7] = 1
# obvs[0, 1, 7, 6] = 1
# obvs[0, 0, 9, 6] = 1
obvs[0, 1, 1:4, 3] = 1
obvs[0, 0, 4, 3] = 1
# obvs[0, 0, 4:8, 6] = 1

mod = model.Model()
mod.load_weight("../weights3/122.pkl")
p, v = mod.evaluate(obvs)
a = np.asarray(p * 100, int)
print(np.resize(a, (15, 15)))
print(v)
# print(np.random.RandomState(seed=10).permutation(10))