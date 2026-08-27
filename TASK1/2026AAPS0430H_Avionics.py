import csv
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

last_correct_depth = None
threshold = 50
X_val = []
Y_val = []

# --- Step 1: read and clean the data first ---
with open('depth_data.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file)

    for row in csv_reader:
        point = float(row['Point'])
        depth = float(row['Depth (m)'])

        if last_correct_depth is not None and abs(depth - last_correct_depth) > threshold:
            depth = last_correct_depth

        X_val.append(point)
        Y_val.append(depth)

        last_correct_depth = depth

# --- Step 2: set up the animated plot using the cleaned data ---
plt.style.use('fivethirtyeight')
fig = plt.figure(figsize=(14, 6))

def animate(i):
    plt.cla()  # clear the current axes
    plt.plot(X_val[:i], Y_val[:i], color="#2F1382", label="Depth",linewidth=1)
    plt.legend(loc='upper left')
    plt.title("Depth data per second")
    plt.xlabel("Point")
    plt.ylabel("Depth (m)")

ani = FuncAnimation(fig, animate, frames=len(X_val), interval=100,repeat=False)

plt.tight_layout()
plt.show()

