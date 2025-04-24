from pathlib import Path
from typing import List
from PIL import Image
import numpy as np
import torch
from tqdm import tqdm
from transformers import CLIPProcessor, CLIPModel

def generate_image_embeddings_with_clip(
        image_paths: List[str | Path],
        model_name: str = "openai/clip-vit-base-patch16",
        batch_size: int = 1
) -> np.ndarray:
    """
    Generate image embeddings using CLIP's vision encoder.

    Args:
        image_paths (List[str | Path]): List of image file paths.
        model_name (str): CLIP model to use. Default is "openai/clip-vit-base-patch16".
        batch_size (int): Batch size for processing. Default is 1.

    Returns:
        np.ndarray: Array of image embeddings (num_images, embedding_dim).
    """
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print("Using device:", device)

    processor = CLIPProcessor.from_pretrained(model_name)
    model = CLIPModel.from_pretrained(model_name).to(device)
    model.eval()

    all_embeddings = []

    for i in tqdm(range(0, len(image_paths), batch_size)):
        batch_paths = image_paths[i:i + batch_size]
        images = [Image.open(path).convert("RGB") for path in batch_paths]

        inputs = processor(images=images, return_tensors="pt", padding=True).to(device)

        with torch.no_grad():
            image_embeds = model.get_image_features(**inputs)

        image_embeds = image_embeds.cpu().numpy()
        all_embeddings.append(image_embeds)

    return np.vstack(all_embeddings).squeeze()
embeddings = generate_image_embeddings_with_clip(path_df.path.tolist(), 
                                       batch_size=16, 
                                       model_name="openai/clip-vit-base-patch16")