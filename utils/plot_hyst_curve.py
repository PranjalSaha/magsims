import pandas as pd
import matplotlib.pyplot as plt
import sys
import pickle

# Load the CSV file
file = sys.argv[1]
df = pd.read_csv(file)

# === Hysteresis Curve ===
fig_hysteresis, ax1 = plt.subplots(figsize=(12, 6))

ax1.plot(df['H(A/m)'], df['B(T)'])

ax1.set_xlabel("H")
ax1.set_ylabel("B")
ax1.set_title("Hysteresis curve")
ax1.legend()
ax1.grid(True)

# Save the figure
with open("../results/hysteresis_trial.pkl", "wb") as f:
    pickle.dump(fig_hysteresis, f)

# === Hysteresis Curve ===
fig_hysteresis_info, ax2 = plt.subplots(figsize=(12, 6))

ax2.plot(df['S.No'], df['H(A/m)'])
ax2.plot(df['S.No'], df['B(T)'])
ax2.plot(df['S.No'], df['slope'])

ax2.set_xlabel("Iteration")
ax2.set_title("Hysteresis info")
ax2.legend()
ax2.grid(True)

# Save the figure
with open("../results/hysteresis_info.pkl", "wb") as f:
    pickle.dump(fig_hysteresis_info, f)

plt.tight_layout()
plt.show()
