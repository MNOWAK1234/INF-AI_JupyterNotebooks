import os
import cv2
import numpy as np
from tqdm import tqdm
import random

dataset_path = r"C:/Users/miknowak/Desktop/Studia/DM/Project1/NamedSignboards"
output_path = dataset_path

os.makedirs(output_path, exist_ok=True)

image_files = [f for f in os.listdir(dataset_path) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]

def random_shift_crop(image, max_shift=20):
    h, w = image.shape[:2]
    tx = random.randint(-max_shift, max_shift)
    ty = random.randint(-max_shift, max_shift)
    start_x = max(0, tx)
    start_y = max(0, ty)
    end_x = min(w, w + tx) if tx < 0 else min(w, w - tx)
    end_y = min(h, h + ty) if ty < 0 else min(h, h - ty)
    if end_x <= start_x or end_y <= start_y:
        return cv2.resize(image, (w, h))
    cropped = image[start_y:end_y, start_x:end_x]
    resized = cv2.resize(cropped, (w, h))
    return resized

def add_random_blur(image):
    ksize = random.choice([1, 3, 5, 7])
    blurred = cv2.GaussianBlur(image, (ksize, ksize), 0)
    return blurred

for img_file in tqdm(image_files):
    img_path = os.path.join(dataset_path, img_file)
    img = cv2.imread(img_path)
    if img is None:
        print(f"Warning: failed to load {img_path}")
        continue
    base_name, ext = os.path.splitext(img_file)
    for idx in range(5):
        shifted_img = random_shift_crop(img)
        blurred_img = add_random_blur(shifted_img)
        suffix = chr(ord('a') + idx)
        new_filename = f"{base_name}{suffix}{ext}"
        save_path = os.path.join(output_path, new_filename)
        cv2.imwrite(save_path, blurred_img)

print("✅ Finished augmenting with shift+crop+blur!")
