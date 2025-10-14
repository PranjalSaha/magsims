import matplotlib.pyplot as plt
import pickle

# Load figure from pickle file
with open("../results/aux_mag_body.pkl", "rb") as f:
    fig = pickle.load(f)

# Display it
plt.show()
