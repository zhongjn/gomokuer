import os
import time
import config as C




def run():
    queue = os.listdir("../weights/eval/queue")
    queue_int = []

    for w in queue:
        queue_int.append(int(w))

    queue_int.sort()

    for w in queue_int:
        best = os.listdir("../weights/best")[0]
        

        best_no = int(best)
        w_no = int(w)
        if w_no <= best_no:
            continue
        print("weight {w} vs {best}".format(w=w_no, best=best_no))
        result_dir = "../weights/eval/result/{0}_{1}".format(w_no, best_no)

        if not os.path.exists(result_dir):
            os.mkdir(result_dir)
        
        for i in range(C.EVAL_PROCESS_COUNT):
            os.system(
                "start ../gomokuer.exe weval -w0 ../weights/{w}.pkl -w1 ../weights/{best}.pkl -r {r} -o {o}".
                format(
                    w=w_no,
                    best=best_no,
                    r=C.EVAL_TARGET_ROUNDS / C.EVAL_PROCESS_COUNT + 20,
                    o=result_dir))
        while True:
            results = os.listdir(result_dir)
            if len(results) >= C.EVAL_TARGET_ROUNDS:
                os.system("taskkill /im gomokuer.exe /f")
                win, draw, loss = 0, 0, 0
                for r in results:
                    o = r.split("#")[1]
                    if o == "W":
                        win += 1
                    if o == "D":
                        draw += 1
                    if o == "L":
                        loss += 1
                ratio = win / loss
                logstr = "{0}_{1} WLRatio {2:.3} ".format(w_no, best_no, ratio)
                print("weight {w} vs {best} result:".format(
                    w=w_no, best=best_no))
                print("win={win}, draw={draw}, loss={loss}".format(
                    win=win, draw=draw, loss=loss))
                print("win/loss={ratio}".format(ratio=ratio))
                if ratio >= 1.2:
                    print("weight {w} become the best weight!".format(w=w_no))
                    os.rename("../weights/best/{0}".format(best_no),
                              "../weights/best/{0}".format(w_no))
                    logstr += "PASS"
                else:
                    print("weight {w} failed.".format(w=w_no))
                    logstr += "FAIL"
                os.remove("../weights/eval/queue/{0}".format(w_no))

                with open("../weights/log/" + logstr, mode="w"):
                    pass

                break
            else:
                print("round {0} of {1}".format(len(results), C.EVAL_TARGET_ROUNDS))
            time.sleep(5)


if __name__ == "__main__":
    run()