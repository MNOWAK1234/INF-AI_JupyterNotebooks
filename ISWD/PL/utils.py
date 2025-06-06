from functools import partial
from typing import Any, Tuple

import numpy as np
import torch
import torch.optim as optim
import torch.nn.functional as F
from sklearn.metrics import roc_auc_score
from torch.utils.data import DataLoader, Dataset
from tqdm import tqdm


class Hook:
    """A class to store a hook for a model."""

    def __init__(self, layer: torch.nn.Module, func: callable):
        """Constructor for the Hook class. Registers a hook on the given model layer.

        Args:
            layer (torch.nn.Module): The layer to register the hook on.
            func (callable): The function to call when the hook is triggered.
        """
        self.hook = layer.register_forward_hook(partial(func, self))

    def remove(self):
        """
        Remove the hook from the model.
        """
        self.hook.remove()

    def __del__(self):
        """Destructor for the Hook class. Removes the hook from the model."""
        self.remove()


def append_output(hook: Hook, mod: torch.nn.Module, input: Any, output: torch.Tensor):
    """Append the output of the layer to the hook."""
    if not hasattr(hook, "stats"):
        hook.stats = []
    if not hasattr(hook, "name"):
        hook.name = mod.__class__.__name__
    data = hook.stats
    data.append(output.data)


def get_simple_input(val: float, num_criteria: int) -> torch.Tensor:
    """
    Creates a simple input tensor with the provided value for all criteria.

    Args:
        val (float): The value to fill the tensor with.
        num_criteria (int): The number of criteria (dimensions) in the tensor.

    Returns:
        torch.Tensor: The created input tensor with shape (1, 1, num_criteria).
    """

    input_tensor = torch.full((1, 1, num_criteria), val, dtype=torch.float)
    return input_tensor.cpu()


class NumpyDataset(Dataset):
    """A class to create a PyTorch dataset from numpy arrays."""

    def __init__(self, data: np.array, targets: np.array):
        """Constructor for the NumpyDataset class.

        Args:
            data (np.array): Input data (features).
            targets (np.array): Target data (labels).
        """
        self.data = torch.Tensor(data)
        self.targets = torch.LongTensor(targets.astype(int))

    def __getitem__(self, index: int) -> Tuple[torch.Tensor, torch.Tensor]:
        """Get the item at the given index.

        Args:
            index (int): The index of the item to get.

        Returns:
            tuple(torch.Tensor, torch.Tensor): The input data and target data at the given index.
        """
        x = self.data[index]
        y = self.targets[index]
        return x, y

    def __len__(self) -> int:
        """Get the length of the dataset."""
        return len(self.data)


def CreateDataLoader(data: np.array, targets: np.array) -> DataLoader:
    """Create a DataLoader from the given data and targets.

    Args:
        data (np.array): Input data (features).
        targets (np.array): Target data (labels).

    Returns:
        DataLoader: A DataLoader containing the given data and targets.
    """
    dataset = NumpyDataset(data, targets)
    return DataLoader(dataset, batch_size=len(dataset))


def Regret(output: torch.FloatTensor, target: torch.LongTensor) -> torch.FloatTensor:
    """Calculate the regret loss between the output and target tensors.
    For each alternative that is in class 1 the output should be positive, so negative values are penalized.
    For each alternative that is in class 0 the output should be negative, so positive values are penalized.

    Args:
        output (torch.FloatTensor): Output tensor from the model.
        target (torch.LongTensor): Target tensor.

    Returns:
        torch.FloatTensor: The regret loss between the output and target tensors.
    """
    return torch.mean(
        F.relu(-(target >= 1).float() * output) + F.relu((target < 1).float() * output)
    )


def Accuracy(output: torch.FloatTensor, target: torch.LongTensor) -> torch.FloatTensor:
    """Calculate the accuracy of the model output compared to the target tensor.

    Args:
        output (torch.FloatTensor): Output tensor from the model.
        target (torch.LongTensor): Target tensor.

    Returns:
        torch.FloatTensor: The accuracy of the model output compared to the target tensor.
    """
    return (target == (output > 0) * 1).detach().numpy().mean()


def AUC(output: torch.FloatTensor, target: torch.LongTensor) -> torch.FloatTensor:
    """Calculate the AUC score of the model output compared to the target tensor.

    Args:
        output (torch.FloatTensor): Output tensor from the model.
        target (torch.LongTensor): Target tensor.

    Returns:
        torch.FloatTensor: The AUC score of the model output compared to the target tensor.
    """
    return roc_auc_score(target.detach().numpy(), output.detach().numpy())


class ScoreTracker:
    def __init__(self):

        self.losses = []
        self.auc_scores = []
        self.acc_scores = []

    def append(self, loss: float, auc: float, acc: float) -> None:
        """
        Append the given loss, auc, and acc scores to the respective lists.

        Args:
            loss (float): The loss score to append.
            auc (float): The AUC score to append.
            acc (float): The accuracy score to append.
        """
        self.losses.append(loss)
        self.auc_scores.append(auc)
        self.acc_scores.append(acc)

    def add(self, outputs: torch.FloatTensor, labels: torch.LongTensor) -> None:
        """Calculate and append the loss, auc, and acc scores for the given model outputs
            and ground truth labels.

        Args:
            outputs (torch.FloatTensor): The model outputs.
            labels (torch.LongTensor): The ground truth labels.
        """
        self.losses.append(Regret(outputs, labels).item())
        self.auc_scores.append(AUC(outputs, labels).item())
        self.acc_scores.append(Accuracy(outputs, labels).item())


def Train(
    model: torch.nn.Module,
    train_dataloader: DataLoader,
    test_dataloader: DataLoader,
    path: str,
    lr: float = 0.01,
    epoch_nr: int = 200,
    slope_decrease: bool = False,
) -> Tuple[float, float, ScoreTracker, ScoreTracker]:
    """Train the given model using the given training and test dataloaders.

    Args:
        model (torch.nn.Module): The model to train.
        train_dataloader (DataLoader): The dataloader containing the training data.
        test_dataloader (DataLoader): The dataloader containing the test data.
        path (str): The path to save the model.
        lr (float, optional): The learning rate for the optimizer. Defaults to 0.01.
        epoch_nr (int, optional): The number of epochs to train the model. Defaults to 200.
        slope_decrease (bool, optional): Whether to decrease the slope of the leaky hard sigmoid.

    Returns:
        tuple(float, float, ScoreTracker, ScoreTracker): A tuple containing the best accuracy,
            best AUC score, training stats, and test stats.
    """
    optimizer = optim.AdamW(
        model.parameters(), lr=lr, betas=(0.9, 0.99), weight_decay=0.01
    )
    # Add a learninge rate scheduler to the optimizer
    scheduler = torch.optim.lr_scheduler.OneCycleLR(
        optimizer, max_lr=lr, steps_per_epoch=len(train_dataloader), epochs=epoch_nr
    )
    best_acc = 0.0
    best_auc = 0.0
    stats_train = ScoreTracker()
    stats_test = ScoreTracker()
    # Decrease the slope of the leaky hard sigmoid activation function in each epoch
    slopes = np.linspace(0.01, 0.003, epoch_nr)
    for epoch in tqdm(range(epoch_nr)):
        if slope_decrease:
            model.set_slope(slopes[epoch])
        for _, data in enumerate(train_dataloader, 0):
            inputs, labels = data
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = Regret(outputs, labels)
            loss.backward()
            optimizer.step()
            scheduler.step()
            acc = Accuracy(outputs, labels)
            auc = AUC(outputs, labels)
            stats_train.append(loss.item(), auc.item(), acc.item())
        with torch.no_grad():
            for _, data in enumerate(test_dataloader, 0):
                inputs, labels = data
                outputs = model(inputs)
                stats_test.add(outputs, labels)

        # Save the model if the accuracy is better than the previous best
        if acc > best_acc:
            best_acc = acc
            best_auc = auc

            torch.save(
                {
                    "epoch": epoch,
                    "model_state_dict": model.state_dict(),
                    "optimizer_state_dict": optimizer.state_dict(),
                },
                path,
            )

    return (
        best_acc,
        best_auc,
        stats_train,
        stats_test,
    )
