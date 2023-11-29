#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import division             # Division in Python 2.7
import matplotlib
matplotlib.use('Agg')                       # So that we can render files without GUI
import matplotlib.pyplot as plt
from matplotlib import rc
import numpy as np
import math

from matplotlib import colors

def plot_color_gradients(gradients, names):
    # For pretty latex fonts (commented out, because it does not work on some machines)
    #rc('text', usetex=True) 
    #rc('font', family='serif', serif=['Times'], size=10)
    rc('legend', fontsize=10)

    column_width_pt = 400         # Show in latex using \the\linewidth
    pt_per_inch = 72
    size = column_width_pt / pt_per_inch

    fig, axes = plt.subplots(nrows=len(gradients), sharex=True, figsize=(size, 0.75 * size))
    fig.subplots_adjust(top=1.00, bottom=0.05, left=0.25, right=0.95)


    for ax, gradient, name in zip(axes, gradients, names):
        # Create image with two lines and draw gradient on it
        img = np.zeros((2, 1024, 3))
        for i, v in enumerate(np.linspace(0, 1, 1024)):
            img[:, i] = gradient(v)

        im = ax.imshow(img, aspect='auto')
        im.set_extent([0, 1, 0, 1])
        ax.yaxis.set_visible(False)

        pos = list(ax.get_position().bounds)
        x_text = pos[0] - 0.25
        y_text = pos[1] + pos[3]/2.
        fig.text(x_text, y_text, name, va='center', ha='left', fontsize=10)

    fig.savefig('my-gradients.pdf')

def rgb_to_hsv(r, g, b):
    max_value = max(r, g, b)
    min_value = min(r, g, b)
    delta = max_value - min_value
    # Hue
    if delta == 0:
        hue = 0
    elif max_value == r:
        hue = 60 * (0 + (g - b) / delta)
    elif max_value == g:
        hue = 60 * (2 + (b - r) / delta)
    else:
        hue = 60 * (4 + (r - g) / delta)
    if hue < 0:
        hue += 360

    # Calculate Saturation
    if max_value == 0:
        saturation = 0
    else:
        saturation = delta / max_value

    # Calculate Value
    value = 100 * max_value

    return hue, saturation, value

def hsv2rgb(h, s, v):
    if (s == 0):
        return v, v, v
    hi = h // 60
    f = h / 60 - hi
    p = v * (1 - s)
    q = v * (1 - (s * f))
    t = v * (1 - (s * (1 - f)))
    if hi == 0:
        r, g, b = v, t, p
    elif hi == 1:
        r, g, b = q, v, p
    elif hi == 2:
        r, g, b = p, v, t
    elif hi == 3:
        r, g, b = p, q, v
    elif hi == 4:
        r, g, b = t, p, v
    else:
        r, g, b = v, p, q
    return r, g, b

def gradient_rgb_bw(v):
    return (v, v, v)

def gradient_rgb_gbr(v):
    r = max(0, (v - 0.5) * 2)  # Red rises from 0.5
    g = max(0, (0.5 - v) * 2)  # Green falls until 0.5
    b = max(0, 1 - abs(0.5 - v) * 2)  # Blue rises from 0 to 0.5 and then falls
    return (r, g, b)

def gradient_rgb_gbr_full(v):
    if (v < 0.25):
        r, g, b = 0, 1, v * 4
    elif (v < 0.5):
        r, g, b = 0, (0.5 - v) * 4, 1
    elif (v < 0.75):
        r, g, b = (v - 0.5) * 4, 0, 1
    else:
        r, g, b = 1, 0, (1 - v) * 4
    return (r, g, b)

def gradient_rgb_wb_custom(v):
    if (v < 1.0 / 7):
        r, g, b = 1, (1.0 / 7 - v) * 7, 1
    elif (v < 2.0 / 7):
        r, g, b = (2.0 / 7 - v) * 7, 0, 1
    elif (v < 3.0 / 7):
        r, g, b = 0, (v - 2.0 / 7) * 7, 1
    elif (v < 4.0 / 7):
        r, g, b = 0, 1, (4.0 / 7 - v) * 7
    elif (v < 5.0 / 7):
        r, g, b = (v - 4.0 / 7) * 7, 1, 0
    elif (v < 6.0 / 7):
        r, g, b = 1, (6.0 / 7 - v) * 7, 0
    else:
        r, g, b = (7.0 / 7 - v) * 7, 0, 0
    return (r, g, b)

def gradient_hsv_bw(v):
    hue = 0  # Hue (0-360 degrees)
    saturation = 0  # Saturation (0-100%)
    value = v # Value (0-100%)
    return hsv2rgb(hue, saturation, value)

def gradient_hsv_gbr(v):
    hue = 120 + (360 - 120) * v # Hue (0-360 degrees)
    saturation = 1  # Saturation (0-100%)
    value = 1  # Value (0-100%)
    return hsv2rgb(hue, saturation, value)

def gradient_hsv_unknown(v):
    hue = 120 * (1 - v) # Hue (0-360 degrees)
    saturation = 0.5  # Saturation (0-100%)
    value = 1  # Value (0-100%)
    return hsv2rgb(hue, saturation, value)

def gradient_hsv_custom(v):
    hue = v * 360 # Hue (0-360 degrees)
    saturation = 1 - v * v  # Saturation (0-100%)
    value = 1 # Value (0-100%)
    return hsv2rgb(hue, saturation, value)

if __name__ == '__main__':
    def toname(g):
        return g.__name__.replace('gradient_', '').replace('_', '-').upper()

    gradients = (gradient_rgb_bw, gradient_rgb_gbr, gradient_rgb_gbr_full, gradient_rgb_wb_custom,
                 gradient_hsv_bw, gradient_hsv_gbr, gradient_hsv_unknown, gradient_hsv_custom)

    plot_color_gradients(gradients, [toname(g) for g in gradients])
