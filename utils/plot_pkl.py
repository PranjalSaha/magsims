import pickle
import matplotlib.pyplot as plt
import pandas as pd
import os
from tkinter import Tk, filedialog


def choose_file():
    root = Tk()
    root.withdraw()  # Hide the main Tkinter window
    file_path = filedialog.askopenfilename(
        title="Select a Pickled File",
        filetypes=[("Pickle files", "*.pkl"), ("All files", "*.*")]
    )
    return file_path


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

        if isinstance(data, pd.DataFrame):
            df = data
            if df.shape[1] < 2:
                print("DataFrame must have at least two columns.")
                return

            columns = df.columns.tolist()
            x = df[columns[0]]

            plt.figure(figsize=(10, 6))
            for col in columns[1:]:
                plt.plot(x, df[col], label=col)

            plt.xlabel(columns[0])
            plt.ylabel("Values")
            plt.title(f"Graph from {os.path.basename(filename)}")
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.show()

        elif isinstance(data, dict):
            plt.figure(figsize=(10, 6))
            for key, values in data.items():
                if isinstance(values, (list, pd.Series)):
                    plt.plot(values, label=str(key))
                else:
                    print(f"Skipping key '{key}': unsupported data type.")
            plt.title(f"Graph from {os.path.basename(filename)}")
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.show()

        elif isinstance(data, list):
            plt.figure(figsize=(10, 6))
            plt.plot(data, label='List Data')
            plt.title(f"Graph from {os.path.basename(filename)}")
            plt.legend()
            plt.grid(True)
            plt.tight_layout()
            plt.show()

        else:
            print(f"Unsupported data type: {type(data)}")

    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    plot_pickled_file()
