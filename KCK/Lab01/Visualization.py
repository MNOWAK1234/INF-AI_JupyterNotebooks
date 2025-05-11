#!/usr/bin/env python3
# -*- coding: utf-8 -*- 

import numpy as np
from matplotlib import pyplot as plt
import csv

# Open and read data from CSV files
file_names = ["1ers.csv", "1crs.csv", "2crs.csv", "1c.csv", "2c.csv"]
markers = ['o', 'v', 'D', 's', 'd']
colors = ['blue', 'green', 'red', 'black', 'magenta']
names = ["1-Evol-RS", "1-Coev-RS", "2-Coev-RS", "1-Coev", "2-Coev"]

# Create a grid of subplots (1 row, 2 columns)
fig, axes = plt.subplots(1, 2, figsize=(10, 5.5))

# Subplot 1 (Left)
ax1 = axes[0]
ax1.tick_params(axis='x', direction="in", length=6, which='major', labelsize=12)
ax1.tick_params(axis='y', direction="in", length=6, which='major', labelsize=12)
ax1.set_xlabel('Rozegranych gier (' + r'$ \times $' + '1000)', fontsize=12)
ax1.set_ylabel('Odsetek wygranych gier [%]', fontsize=12)
ax1.set_xlim(0, 500)
ax1.set_ylim(60, 100)  # Set the y-axis limits to 60-100
# Add a second x-axis
ax1_top = ax1.twiny()
ax1_top.set_xlim(0, 500)
ax1_top.set_xticks([0, 100, 200, 300, 400, 500])
ax1_top.set_xticklabels(["0", "40", "80", "120", "160", "200"])
ax1_top.set_xlabel("Pokolenie")
ax1_top.tick_params(direction='in')

boxplot_data = []

# Process the CSV files and plot the data for Subplot 1
for i, file_name in enumerate(file_names):
    with open(file_name, 'r') as file:
        generations, efforts, average_results = [], [], []
        first_line = True
        last_line = None
        for line in file:
            if first_line:
                first_line = False
                continue
            separated_line = line.strip().split(",")
            generations.append(int(separated_line[0]))
            effort = float(separated_line[1]) / 1000
            efforts.append(effort)
            average_result = np.mean([float(value) for value in separated_line[2:]])
            average_results.append(100 * average_result)

            last_line = line  # Store the last line for Subplot 2

        label = names[i]
        ax1.plot(efforts, average_results, marker=markers[i], markevery=25, markeredgecolor='black', color=colors[i], label=label)

        separated_line = last_line.strip().split(",")
        boxplot_data.append([float(value) * 100 for value in separated_line[2:]])

# Create a new set of axes for Subplot 2 (Right)
ax2 = axes[1]
ax2.tick_params(axis='y', direction="in", length=6, which='major', labelsize=12)
ax2.set_ylim(60, 100)  # Set the y-axis limits to 60-100
ax2.yaxis.tick_right()  # Move the y-axis to the right
ax2.tick_params(direction='in')
meanprops = dict(marker='o',mfc='blue',mec='black')
medianprops=dict(linestyle='-',c='red')
whiskerprops=dict(linestyle=(0,(7,10)),c='blue')
boxprops=dict(linestyle='-',c='blue')
# Create boxplots on Subplot 2
ax2.boxplot(boxplot_data, 1,'b+', vert=True, labels=names, showmeans=True, meanprops=meanprops, medianprops=medianprops, whiskerprops=whiskerprops, boxprops=boxprops)

# Show the legend for the data from different files in Subplot 1
ax1.legend(fontsize=12, loc='lower right', frameon=True)

# Add grids to both subplots
ax1.grid(linestyle=':')
ax2.grid(linestyle=':')

# Adjust the layout to prevent overlap
plt.tight_layout()

# Save the figure to a file
plt.savefig('wizualizacja_LAB1.pdf')

plt.show()

plt.close()
