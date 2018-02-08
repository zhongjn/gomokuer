import numpy as np
import model
obvs = np.zeros([1, 2, 15, 15], float)
obvs[0, 0, 7, 7:10] = 1
obvs[0, 1, 7, 6] = 1

# obvs[0, 0, 4:8, 6] = 1

mod = model.Model()
mod.load_weight("../weights/22.pkl")
p, v = mod.evaluate(obvs)
a = np.asarray(p * 100, int)
print(np.resize(a, (15, 15)))
print(v)
# print(np.random.RandomState(seed=10).permutation(10))