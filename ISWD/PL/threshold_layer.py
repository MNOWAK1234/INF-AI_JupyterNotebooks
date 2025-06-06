import torch
import torch.nn as nn


class ThresholdLayer(nn.Module):
    """A threshold layer that subtracts a threshold from the input tensor.
    Alternatives from class 1 should have utility greater than the threshold.
    Alternatives from class 0 should have utility less than the threshold.

    """

    def __init__(self, threshold: float = None, requires_grad: bool = True):
        """Initialize the threshold layer.

        Args:
            threshold (float, optional): The threshold value. Defaults to None.
                If None, the threshold is initialized randomly.
            requires_grad (bool, optional): Whether the threshold should be trainable.
                Defaults to True.
        """
        super().__init__()
        if threshold is None:
            self.threshold = nn.Parameter(
                torch.FloatTensor(1).uniform_(0.1, 0.9), requires_grad=requires_grad
            )
        else:
            self.threshold = nn.Parameter(
                torch.FloatTensor([threshold]), requires_grad=requires_grad
            )

    def forward(self, x: torch.FloatTensor) -> torch.FloatTensor:
        """Forward pass of the threshold layer.

        Args:
            x (torch.FloatTensor): Input tensor.

        Returns:
            torch.FloatTensor: Output tensor.
        """
        return x - self.threshold
