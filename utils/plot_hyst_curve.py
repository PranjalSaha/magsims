#!/bin/python3

# import matplotlib as mpl
import pandas as pd
import matplotlib.pyplot as plt
import sys

# Load the CSV file
file = sys.argv[1]

# Fix for Agg OverflowError
# mpl.rcParams["agg.path.chunksize"] = 10000


def plotBH(csvFile):
    data = pd.read_csv(csvFile)

    sno = data.iloc[:, 0]
    H = data.iloc[:, 1]
    B = data.iloc[:, 2]

    # -------- Plot 1: B(T) vs H(A/m) --------
    plt.figure()
    plt.plot(H, B)
    plt.xlabel("H (A/m)")
    plt.ylabel("B (T)")
    plt.title("B vs H")
    plt.grid(True)

    # -------- Plot 2: H(A/m) and B(T) vs S.No --------
    plt.figure()
    plt.plot(sno, H, label="H (A/m)")
    plt.plot(sno, B, label="B (T)")
    plt.xlabel("S.No")
    plt.ylabel("H (A/m) and B (T)")
    plt.title("H and B vs S.No")
    plt.legend()
    plt.grid(True)

    # plotting all graphs together
    plt.show()


if __name__ == "__main__":
    plotBH(file)
