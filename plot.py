import pandas as pd
import matplotlib.pyplot as plt

import sys

def main(args):
    df = pd.read_csv(args[0])

    out_file = args[1]

    series = df.columns[2:]

    for s in series:
        plt.scatter(df["input_file"], df[s], label=s)

    # plt.xticks(rotation=90)
    
    plt.legend()
    plt.xlabel("input_file")
    plt.ylabel("execution time (s)")
    plt.savefig(f"{out_file}.png", bbox_inches='tight')

    plt.xscale("log")
    plt.yscale("log")
    plt.savefig(f"{out_file}-log.png", bbox_inches='tight')

if __name__ == "__main__":
    main(sys.argv[1:])
