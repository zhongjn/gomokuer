import uuid
import numpy as np
import os
import zipfile


def save_samples(observation, prob, result):
    np.savez(
        "../games/raw/{0}".format(uuid.uuid1()),
        observation=observation,
        prob=prob,
        result=result)


def save_eval_result(outdir, result):
    if os.path.exists(outdir):
        with open("{o}/{uuid}#{r}".format(o=outdir, uuid=uuid.uuid1(), r=result), mode="w"):
            pass
    else:
        print("cannot find outdir!")


def make_zip(source_dir, output_filename):
    zipf = zipfile.ZipFile(output_filename, 'w')
    pre_len = len(os.path.dirname(source_dir))
    for parent, dirnames, filenames in os.walk(source_dir):
        for filename in filenames:
            pathfile = os.path.join(parent, filename)
            arcname = pathfile[pre_len:].strip(os.path.sep)
            zipf.write(pathfile, arcname)
    zipf.close()