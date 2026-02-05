#!/bin/python3


import pickle
import pandas as pd
import matplotlib.pyplot as plt
import sys
import numpy as np


# === CONFIGURATION ===
MAX_ROWS_TO_READ = 10000
LARGE = False

# Figure size for all plots
FIGURE_SIZE = (12, 6)

# Line styling
LINE_WIDTH = 0.5
LINE_STYLE = '-'
ALPHA = 0.8

# Grid and layout settings
GRID_ENABLED = True
GRID_ALPHA = 0.3
LEGEND_FONTSIZE = 10
AXIS_LABEL_FONTSIZE = 12
TITLE_FONTSIZE = 14

# Color palette for x, y, z components
COLOR_PALETTE = ['#1f77b4', '#ff7f0e', '#2ca02c']  # Blue, Orange, Green

# Results directory
FILE = sys.argv[1]
FILE_PRFX = ".." + FILE.split('.')[-2] + '_'

'''
# === READING CSV FILES ===

# Count total lines (excluding header)
with open(FILE, 'r') as f:
    total_lines = sum(1 for _ in f) - 1  # minus header

# Downsampling the data into smaller value
if total_lines > MAX_ROWS_TO_READ:
    STEP = int(np.ceil(total_lines / MAX_ROWS_TO_READ))
    print(f"Large file detected: {total_lines:,} lines. "
          f"Reading every {STEP}th row.")

else:
    print(f"File size manageable: {total_lines:,} lines. Reading all rows.")
    STEP = 0


def skip_rows(x):
    if STEP == 0:
        return True
    else:
        return (x-1) % STEP == 0 or x == 0


# Read file with skipping
df = pd.read_csv(FILE, skiprows=skip_rows)

# Clean up column names and convert time
df.columns = df.columns.str.strip()
df["Time"] = pd.to_datetime(
    df["Time"],
    format="%d %b %Y %H:%M:%S.%f",
    errors="raise"
)
    '''

'''
with open(FILE, 'r') as f:
    total_lines = sum(1 for _ in f) - 1  # minus header

if total_lines > MAX_ROWS_TO_READ:
    step = int(np.ceil(total_lines / MAX_ROWS_TO_READ))
    print(f"Sampling every {step}th row...")
    LARGE = True

    def skip_rows(i):
        # Always keep header
        if i == 0:
            return False
        # Keep every step-th data row
        return (i - 1) % step != 0

    df = pd.read_csv(FILE, skiprows=skip_rows)

else:
    df = pd.read_csv(FILE)
    '''

# === READING CSV FILES ===

# Count total lines (excluding header)
file = sys.argv[1]
print(file)
with open(file, 'r') as f:
    total_lines = sum(1 for _ in f) - 1  # minus header

# Downsampling the data into smaller value
if total_lines > MAX_ROWS_TO_READ:
    STEP = int(np.ceil(total_lines / MAX_ROWS_TO_READ))
    print(f"Large file detected: {total_lines:,} lines. "
          f"Reading every {STEP}th row.")

else:
    print(f"File size manageable: {total_lines:,} lines. Reading all rows.")
    STEP = 0


def skip_rows(x):
    if STEP == 0:
        return False
    return x % STEP != 0 and x != 0


# Read file with skipping
df = pd.read_csv(file, skiprows=skip_rows)

# Clean up column names and convert time
df.columns = df.columns.str.strip()
df["Time"] = pd.to_datetime(
    df["Time"],
    format="%d %b %Y %H:%M:%S.%f",
    errors="raise"
)

# === First Figure: Auxiliary Magnetic Field ===
fig_aux_mag_body, ax1 = plt.subplots(figsize=FIGURE_SIZE)

col_aux_mag_body = [
    'aux_mag_body_x(A/m)',
    'aux_mag_body_y(A/m)',
    'aux_mag_body_z(A/m)'
]

for i, col in enumerate(col_aux_mag_body):
    ax1.plot(df["Time"], df[col], label=col, linewidth=LINE_WIDTH,
             linestyle=LINE_STYLE, alpha=ALPHA, color=COLOR_PALETTE[i])

ax1.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax1.set_ylabel("Magnetic Field (A/m)", fontsize=AXIS_LABEL_FONTSIZE)
ax1.set_title("Auxiliary Magnetic Field Components Over Time in Body Frame",
              fontsize=TITLE_FONTSIZE)
ax1.legend(fontsize=LEGEND_FONTSIZE)
ax1.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the figure
with open(f"{FILE_PRFX}aux_mag_body.pkl", "wb") as f:
    pickle.dump(fig_aux_mag_body, f)


# === Second Figure: Angular Velocity (Inertial Frame) ===
fig_ang_vel_inrt, ax2 = plt.subplots(figsize=FIGURE_SIZE)

col_ang_vel_inrt = [
    'ang_vel_inrt_x(rad/s)',
    'ang_vel_inrt_y(rad/s)',
    'ang_vel_inrt_z(rad/s)'
]

for i, col in enumerate(col_ang_vel_inrt):
    ax2.plot(df["Time"], df[col], label=col, linewidth=LINE_WIDTH,
             linestyle=LINE_STYLE, alpha=ALPHA, color=COLOR_PALETTE[i])

ax2.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax2.set_ylabel("Angular Velocity (rad/s)", fontsize=AXIS_LABEL_FONTSIZE)
ax2.set_title("Angular Velocity (Inertial Frame) Over Time",
              fontsize=TITLE_FONTSIZE)
ax2.legend(fontsize=LEGEND_FONTSIZE)
ax2.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the figure
with open(f"{FILE_PRFX}ang_vel_inrt.pkl", "wb") as f:
    pickle.dump(fig_ang_vel_inrt, f)


# === Third Figure: Angular Velocity (Body Frame) ===
fig_ang_vel_body, ax3 = plt.subplots(figsize=FIGURE_SIZE)

col_ang_vel_body = [
    'ang_vel_body_x(rad/s)',
    'ang_vel_body_y(rad/s)',
    'ang_vel_body_z(rad/s)'
]

for i, col in enumerate(col_ang_vel_body):
    ax3.plot(df["Time"], df[col], label=col, linewidth=LINE_WIDTH,
             linestyle=LINE_STYLE, alpha=ALPHA, color=COLOR_PALETTE[i])

ax3.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax3.set_ylabel("Angular Velocity (rad/s)", fontsize=AXIS_LABEL_FONTSIZE)
ax3.set_title("Angular Velocity (Body Frame) Over Time",
              fontsize=TITLE_FONTSIZE)
ax3.legend(fontsize=LEGEND_FONTSIZE)
ax3.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the figure
with open(f"{FILE_PRFX}ang_vel_body.pkl", "wb") as f:
    pickle.dump(fig_ang_vel_body, f)


# === Third Figure: Angular Velocity Magnitude ===
fig_ang_vel_mag, ax3 = plt.subplots(figsize=FIGURE_SIZE)

col_ang_vel_body = [
    'ang_vel_inrt_m(rad/s)'
]

for i, col in enumerate(col_ang_vel_body):
    ax3.plot(df["Time"], df[col], label=col, linewidth=LINE_WIDTH,
             linestyle=LINE_STYLE, alpha=ALPHA, color=COLOR_PALETTE[i])

ax3.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax3.set_ylabel("Angular Velocity (rad/s)", fontsize=AXIS_LABEL_FONTSIZE)
ax3.set_title("Angular Velocity (Magnitude) Over Time",
              fontsize=TITLE_FONTSIZE)
ax3.legend(fontsize=LEGEND_FONTSIZE)
ax3.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the figure
with open(f"{FILE_PRFX}ang_vel_mag.pkl", "wb") as f:
    pickle.dump(fig_ang_vel_mag, f)


# === Fourth Figure: Hysteresis Curve ===
fig_hysteresis, ax4 = plt.subplots(figsize=FIGURE_SIZE)

col_hysteresis = [
    'hys_mag_body_x(Tesla)',
    'hys_mag_body_y(Tesla)',
    'hys_mag_body_z(Tesla)'
]

row_hysteresis = [
    'aux_mag_body_x(A/m)',
    'aux_mag_body_y(A/m)',
    'aux_mag_body_z(A/m)'
]

labels = ['x', 'y', 'z']

for i in range(3):
    ax4.plot(df[row_hysteresis[i]], df[col_hysteresis[i]], label=labels[i],
             linewidth=LINE_WIDTH, linestyle=LINE_STYLE, alpha=ALPHA,
             color=COLOR_PALETTE[i])

ax4.set_xlabel("H (A/m)", fontsize=AXIS_LABEL_FONTSIZE)
ax4.set_ylabel("B (Tesla)", fontsize=AXIS_LABEL_FONTSIZE)
ax4.set_title("Hysteresis Curves", fontsize=TITLE_FONTSIZE)
ax4.legend(fontsize=LEGEND_FONTSIZE)
ax4.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the fourth figure
with open(f"{FILE_PRFX}hysteresis.pkl", "wb") as f:
    pickle.dump(fig_hysteresis, f)


# === Fifth Figure: Torque (Inertial) ===
fig_torque_inrt, ax5 = plt.subplots(figsize=FIGURE_SIZE)

col_torque_inrt = [
    'torque_x_inrt(Nm)',
    'torque_y_inrt(Nm)',
    'torque_z_inrt(Nm)',
]

labels = ['x', 'y', 'z']

for i in range(3):
    ax5.plot(df["Time"], df[col_torque_inrt[i]], label=labels[i],
             linewidth=LINE_WIDTH, linestyle=LINE_STYLE, alpha=ALPHA,
             color=COLOR_PALETTE[i])

ax5.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax5.set_ylabel("Torque (Nm)", fontsize=AXIS_LABEL_FONTSIZE)
ax5.set_title("Torque (Inertial Frame)", fontsize=TITLE_FONTSIZE)
ax5.legend(fontsize=LEGEND_FONTSIZE)
ax5.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the fifth figure
with open(f"{FILE_PRFX}torque_inrt.pkl", "wb") as f:
    pickle.dump(fig_torque_inrt, f)


# === Sixth Figure: Torque (Body) ===
fig_torque_body, ax6 = plt.subplots(figsize=FIGURE_SIZE)

col_torque_body = [
    'torque_x_body(Nm)',
    'torque_y_body(Nm)',
    'torque_z_body(Nm)',
]

labels = ['x', 'y', 'z']

for i in range(3):
    ax6.plot(df["Time"], df[col_torque_body[i]], label=labels[i],
             linewidth=LINE_WIDTH, linestyle=LINE_STYLE, alpha=ALPHA,
             color=COLOR_PALETTE[i])

ax6.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax6.set_ylabel("Torque (Nm)", fontsize=AXIS_LABEL_FONTSIZE)
ax6.set_title("Torque (Body Frame)", fontsize=TITLE_FONTSIZE)
ax6.legend(fontsize=LEGEND_FONTSIZE)
ax6.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the sixth figure
with open(f"{FILE_PRFX}torque_body.pkl", "wb") as f:
    pickle.dump(fig_torque_body, f)


# === Seventh Figure: Magnetic Moment (Body) ===
fig_mmt_body, ax7 = plt.subplots(figsize=FIGURE_SIZE)

col_mmt_body = [
    'mag_mmt_body_x(Am2)',
    'mag_mmt_body_y(Am2)',
    'mag_mmt_body_z(Am2)',
]

labels = ['x', 'y', 'z']

for i in range(3):
    ax7.plot(df["Time"], df[col_mmt_body[i]], label=labels[i],
             linewidth=LINE_WIDTH, linestyle=LINE_STYLE, alpha=ALPHA,
             color=COLOR_PALETTE[i])

ax7.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax7.set_ylabel("Magnetic Moment (Am²)", fontsize=AXIS_LABEL_FONTSIZE)
ax7.set_title("Magnetic Moment (Body Frame)", fontsize=TITLE_FONTSIZE)
ax7.legend(fontsize=LEGEND_FONTSIZE)
ax7.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the seventh figure
with open(f"{FILE_PRFX}mag_mmt_body.pkl", "wb") as f:
    pickle.dump(fig_mmt_body, f)


# === Eighth Figure: Magnetic Moment (Inertial Frame) ===
fig_mmt_inrt, ax8 = plt.subplots(figsize=FIGURE_SIZE)

col_mmt_inrt = [
    'mag_mmt_inrt_x(Am2)',
    'mag_mmt_inrt_y(Am2)',
    'mag_mmt_inrt_z(Am2)',
]

labels = ['x', 'y', 'z']

for i in range(3):
    ax8.plot(df["Time"], df[col_mmt_inrt[i]], label=labels[i],
             linewidth=LINE_WIDTH, linestyle=LINE_STYLE, alpha=ALPHA,
             color=COLOR_PALETTE[i])

ax8.set_xlabel("Time", fontsize=AXIS_LABEL_FONTSIZE)
ax8.set_ylabel("Magnetic Moment (Am²)", fontsize=AXIS_LABEL_FONTSIZE)
ax8.set_title("Magnetic Moment (Inertial Frame)", fontsize=TITLE_FONTSIZE)
ax8.legend(fontsize=LEGEND_FONTSIZE)
ax8.grid(GRID_ENABLED, alpha=GRID_ALPHA)

# Show the plot if file is large
if LARGE is True:
    plt.show()

# Save the eighth figure
with open(f"{FILE_PRFX}mag_mmt_inrt.pkl", "wb") as f:
    pickle.dump(fig_mmt_inrt, f)

# === Show all figures ===
plt.tight_layout()
plt.show()  # Non-blocking show allows multiple windows

# input("Press Enter to close all plots...")
# plt.close('all')
