from collections import defaultdict
import os

import pandas as pd

def main():
    outfiles = [filename.strip(".out") for filename in os.listdir("jobs") if filename.endswith(".out")]

    resmap = defaultdict(dict)
    for filename in outfiles:
        res, impl = filename.split("_")

        with open("jobs/" + filename + ".out", "r") as f:
            time = float(f.readlines()[-1])

        resmap[res][impl] = time

    df = pd.DataFrame(resmap).transpose().sort_values("for")
    df.insert(0, "repetitions", "")
    df["repetitions"] = 1

    df.index.name = "input_file"

    df.to_csv("results/5.csv")


if __name__ == "__main__":
    main()