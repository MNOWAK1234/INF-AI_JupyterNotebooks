import numpy as np
import matplotlib.pyplot as plt

file_path = 'big.dem'

width, height, distance = 0, 0, 0

def custom_sigmoid(x, breakpoint = 0.33, rate = 40):
    sigmoid = 1 / (1 + np.exp(-rate * (x - breakpoint)))
    return sigmoid

#function copied from the previous task
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

#From green (0) to red (1)
def gradientMap(height, incline, angle):
    hue = 120 * (1 - height) # Hue (0-360 degrees)
    modified_angle = custom_sigmoid(angle)
    saturation = modified_angle
    if(modified_angle < 0.25):
        value = 1
    else:
        value = (1 - incline) ** 8  # Value (0-100%)
    return hsv2rgb(hue, saturation, value)

# Lists to store the data
data = []

# Open and read the file
with open(file_path, 'r') as file:
    # Read the first line with width, height, and constant
    width, height, distance = map(int, file.readline().split())

    # Read the remaining lines and store them in the data list
    for _ in range(height):
        row = list(map(float, file.readline().split()))
        data.append(row)

# Convert data to a NumPy array
data_array = np.array(data)

diff_lr = distance
diff_tb = distance 

# Define the vector pointing to the sun (replace with the actual values)
sun_vector = np.array([1, 1, 1])  # Adjust the values as needed

# Create an empty array to store the normal vectors and angles
normal_vectors = np.zeros((height, width, 3))
angles_to_sun = np.zeros((height, width))

# Calculate the normal vectors for each point and its right and bottom neighbors
for y in range(height):
    for x in range(width):
        z = data_array[y, x]  # Get the z-coordinate of the current point
        right_x = x + 1 if x + 1 < width else x  # Avoid going out of bounds
        bottom_y = y + 1 if y + 1 < height else y  # Avoid going out of bounds

        # Calculate vectors to right and bottom neighbors
        vector_to_right = np.array([diff_lr, 0, data_array[y, right_x] - z])
        vector_to_bottom = np.array([0, diff_tb, data_array[bottom_y, x] - z])

        # Calculate the normal vector using the cross product
        normal_vector = np.cross(vector_to_right, vector_to_bottom)

        # Normalize the normal vector
        normal_vector /= np.linalg.norm(normal_vector)

        # Calculate the angle between the normal vector and the vector to the sun
        angle_to_sun = np.arccos(np.dot(normal_vector, sun_vector) / (np.linalg.norm(normal_vector) * np.linalg.norm(sun_vector)))

        # Store the normal vector and angle in their respective arrays
        normal_vectors[y, x] = normal_vector

        # Normalize the angle to the range [0, 1]
        normalized_angle = angle_to_sun / np.pi

        # Store the normalized angle in the 'angles_to_sun' array
        angles_to_sun[y, x] = normalized_angle

min_a = np.min(angles_to_sun)
max_a = np.max(angles_to_sun)

# Find the minimum and maximum values in the array
min_value = np.min(data_array)
max_value = np.max(data_array)

# Linearly interpolate the data to the range [0, 1]
normalized_data = (data_array - min_value) / (max_value - min_value)
normalized_angle_to_sun = (angles_to_sun - min_a) / (max_a - min_a)

#Calculate differences between neighbours
diff_top = np.diff(data_array, axis=0, prepend=0)
diff_bottom = np.diff(data_array, axis=0, append=0)
diff_left = np.diff(data_array, axis=1, prepend=0)
diff_right = np.diff(data_array, axis=1, append=0)

max_down = abs(np.minimum.reduce([diff_top, diff_bottom, diff_left, diff_right]))
max_any = np.maximum.reduce([diff_top, diff_bottom, diff_left, diff_right])

# Find the minimum and maximum values in the array
min_incline = np.min(max_down)
max_incline = np.max(max_down)

# Linearly interpolate the data to the range [0, 1]
normalized_down_incline = (max_down - min_incline) / (max_incline - min_incline)

custom_colors = np.array([
    [
        gradientMap(value, down_incline, angle)
        for value, down_incline, angle in zip(row, row_incline, row_angle)
    ]
    for row, row_incline, row_angle in zip(normalized_data, normalized_down_incline, normalized_angle_to_sun)
])

# Create a figure and display the colored grid
fig, axes = plt.subplots(figsize=(6, 6))

# Add ticks on all sides (left, right, top, and bottom)
axes.tick_params(axis='x', direction="in", length=6, which='both', labelsize=12, bottom=True, top=True)
axes.tick_params(axis='y', direction="in", length=6, which='both', labelsize=12, left=True, right=True)

axes.imshow(custom_colors, origin='lower')

# Show the plot
plt.show()