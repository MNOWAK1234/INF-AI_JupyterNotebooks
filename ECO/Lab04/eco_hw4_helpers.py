from random import random

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

plt.style.use("ggplot")


def random_walk(f, x0, steps=10):
    x = np.array(x0)
    points = [x]
    for _ in range(steps):
        x = x + (np.random.random(np.shape(x)) - 0.5) * 2
        points.append(x)
    return points


def generate_1d_regression_data(n=100, seed=123, noise=2):
    np.random.seed(seed)
    x = np.random.random(n) * 2 - 1
    a, b = 2, 1
    y = x * a + b
    if noise:
        y += np.random.normal(0, noise, y.shape)
    return x, y


def mse_loss_1d(x, y, a, b):
    return (0.5 * (x * a + b - y) ** 2).mean()


def plot_losses(losses, ax):
    ax.set_title("error in time")
    ax.set_ylabel('loss')
    ax.set_xlabel('iterations')
    ax.plot(losses, marker="o")


def plot_1d_regression_path(X, Y, points, resolution=100, margin=0.5):
    x_in_time = np.array(np.array(points), dtype=np.float64)
    x_in_time = x_in_time.reshape((len(points), 2))
    function = lambda coeffs: mse_loss_1d(X, Y, coeffs[0], coeffs[1])

    y_in_time = np.array([function(x) for x in x_in_time])

    fig, ax = plt.subplots(nrows=1, ncols=2, figsize=(15, 6.15))
    error_in_time_axis = ax[0]
    gradients_plot = ax[1]

    gradients_plot.set_ylabel('bias')
    gradients_plot.set_xlabel('slope')

    num_ticks = 5
    x0_span = abs(x_in_time[:, 0].max() - x_in_time[:, 0].min())
    x1_span = abs(x_in_time[:, 1].max() - x_in_time[:, 1].min())
    x0_axis_min_lim = x_in_time[:, 0].min() - margin * x0_span
    x0_axis_max_lim = x_in_time[:, 0].max() + margin * x0_span
    x1_axis_min_lim = x_in_time[:, 1].min() - margin * x1_span
    x1_axis_max_lim = x_in_time[:, 1].max() + margin * x1_span

    min_lim = min(x0_axis_min_lim, x1_axis_min_lim)
    max_lim = max(x0_axis_max_lim, x1_axis_max_lim)
    lim_span = max_lim - min_lim

    x0_grid = np.linspace(min_lim, max_lim, resolution)
    x1_grid = np.linspace(min_lim, max_lim, resolution)

    m = np.zeros((resolution, resolution))
    for x0_i, x in enumerate(x0_grid):
        for x1_i, y in enumerate(x1_grid):
            m[x0_i, x1_i] = function(np.array([x, y]))

    x0_ticks = np.linspace(min_lim, max_lim, num_ticks)
    x1_ticks = np.linspace(min_lim, max_lim, num_ticks)
    x0_ticks = ["{:0.2f}".format(x0) for x0 in x0_ticks]
    x1_ticks = ["{:0.2f}".format(x1) for x1 in x1_ticks]

    x_in_time -= min_lim
    x_in_time /= lim_span
    x_in_time[:, 1] = 1 - x_in_time[:, 1]
    x_in_time *= resolution

    # Plot arrows for gradients:
    for ti in range(1, len(x_in_time)):
        # Determine start of an arrow
        x, y = x_in_time[ti]
        # Determine end of an arrow
        old_x, old_y = x_in_time[ti - 1]
        # Plot a red arrow
        gradients_plot.annotate('', xy=(x, y),
                                xytext=(old_x, old_y),
                                arrowprops={'arrowstyle': '->', 'color': 'white', 'lw': 2},
                                va='center', ha='center')
    sns.heatmap(np.flip(m.T, 0), ax=gradients_plot)
    gradients_plot.scatter(*x_in_time[[0, -1]].T, c="white", s=30, lw=0)
    gradients_plot.scatter(*x_in_time[1:-1].T, c="white", s=30, lw=0)

    gradients_plot.set_xticks(np.linspace(0, resolution, num_ticks))
    gradients_plot.set_xticklabels(x0_ticks)
    gradients_plot.set_yticks(np.linspace(resolution, 0, num_ticks))
    gradients_plot.set_yticklabels(x1_ticks)
    gradients_plot.tick_params(axis='x', rotation=0)

    plot_losses(y_in_time, error_in_time_axis)
    plt.axis('equal')
    plt.tight_layout()
    plt.show()


def plot_1d_regression_lines(X, Y, points, lines=5):
    fig, ax = plt.subplots(nrows=1, ncols=2, figsize=(15, 6.15))
    correct_axis = ax[1]
    gd_axis = ax[0]
    sns.regplot(x=X, y=Y, ax=correct_axis)

    gd_axis.scatter(X, Y)
    for i in range(0, len(points), max(len(points) // lines, 1)):
        a, b = points[i]
        y_fit = X * a + b
        gd_axis.plot(X, y_fit, label="t={} ({:0.2f}x + {:0.2f})".format(i, float(a), float(b)))
    gd_axis.legend()

    gd_axis.set_title("computed regression lines")
    correct_axis.set_title('built-in correct regression')
    plt.show()
