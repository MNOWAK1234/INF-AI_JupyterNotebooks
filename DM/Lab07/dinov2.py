
from pathlib import Path
from typing import List
import numpy as np
from transformers import AutoImageProcessor, AutoModel

def generate_image_embeddings_with_dinov2(
        image_paths: List[str | Path],
        model_name: str = "facebook/dinov2-base",
        batch_size: int = 1
) -> np.ndarray:
    """
    Generate embeddings for a batch of images using a pretrained DINOv2 model with CUDA support.

    Args:
        image_paths (List[str | Path]): List of image file paths.
        model_name (str): Name of the pretrained model to use. Default is "facebook/dinov2-base".
        batch_size (int): Number of images to process per batch. Default is 1.

    Returns:
        np.ndarray: 2D array of image embedding vectors (num_images, embedding_dim).
    """
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print("Using device:", device)

    processor = AutoImageProcessor.from_pretrained(model_name)
    model = AutoModel.from_pretrained(model_name).to(device)
    model.eval()

    all_embeddings = []

    for i in tqdm(range(0, len(image_paths), batch_size)):
        batch_paths = image_paths[i:i + batch_size]
        images = [Image.open(path).convert("RGB") for path in batch_paths]

        inputs = processor(images=images, return_tensors="pt").to(device)

        with torch.no_grad():
            outputs = model(**inputs)

        # DINOv2 outputs `last_hidden_state` (batch_size, seq_len, hidden_dim)
        # We mean-pool over the sequence (excluding the [CLS] token if present)
        embeddings = outputs.last_hidden_state.mean(dim=1).cpu().numpy()
        all_embeddings.append(embeddings)


    return np.vstack(all_embeddings).squeeze()
embeddings = generate_image_embeddings_with_dinov2(path_df.path.tolist(), batch_size=16, model_name="facebook/dinov2-base")