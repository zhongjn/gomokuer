import evaluator
import selfplay
import train
import dir_util

dir_util.init_dirs()

while True:
    evaluator.run()
    selfplay.run()
    train.run()