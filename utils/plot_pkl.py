#!/bin/python3

import pickle
import matplotlib.pyplot as plt
import pandas as pd
import os
from tkinter import Tk, filedialog
from matplotlib.figure import Figure
from matplotlib._pylab_helpers import Gcf


def choose_file():
    root = Tk()
    root.withdraw()
    return filedialog.askopenfilename(
        title="Select a Pickled File",
        filetypes=[("Pickle files", "*.pkl"), ("All files", "*.*")]
    )


def plot_pickled_file():
    filename = choose_file()

    if not filename:
        print("No file selected.")
        return

    if not os.path.isfile(filename):
        print(f"File '{filename}' does not exist.")
        return

    try:
        with open(filename, 'rb') as f:
            data = pickle.load(f)

        # --- CASE 1: Pickled Matplotlib Figure ---
        if isinstance(data, Figure):
            print("Detected pickled Matplotlib figure.")
            Gcf.set_active(data.canvas.manager)
            plt.show()
            return

        # --- CASE 2: Pandas DataFrame ---
        if isinstance(data, pd.DataFrame):
            if data.shape[1] < 2:
                print("DataFrame must have at least two columns.")
                return

            columns = data.columns.tolist()
            x = data[columns[0]]

            plt.figure(figsize=(10, 6))
            for col in columns[1:]:
                plt.plot(x, data[col], label=col)

            plt.xlabel(columns[0])
            plt.ylabel("Values")
            plt.title(f"Graph from {os.path.basename(filename)}")
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.show()
            return

        # --- CASE 3: Dictionary of sequences ---
        if isinstance(data, dict):
            plt.figure(figsize=(10, 6))
            for key, values in data.items():
                if isinstance(values, (list, pd.Series)):
                    plt.plot(values, label=str(key))
                else:
                    print(f"Skipping key '{key}': unsupported type")

            plt.title(f"Graph from {os.path.basename(filename)}")
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.show()
            return

        # --- CASE 4: List ---
        if isinstance(data, list):
            plt.figure(figsize=(10, 6))
            plt.plot(data, label="List Data")
            plt.title(f"Graph from {os.path.basename(filename)}")
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.show()
            return

        # --- FALLBACK ---
        print(f"Unsupported pickled type: {type(data)}")

    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    plot_pickled_file()
