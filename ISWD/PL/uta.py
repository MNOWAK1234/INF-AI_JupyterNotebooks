import torch
import torch.nn as nn
from monotonic_layer import MonotonicLayer


class Uta(nn.Sequential):
    """
    Custom module for Uta network.

    Args:
        num_criteria (int): Number of criteria.
        num_hidden_components (int): Number of hidden components of monotonic block.
        slope (float, optional): Slope value for LeakyHardSigmoid. Defaults to 0.01.
    """

    def __init__(
        self,
        num_criteria: int,
        num_hidden_components: int,
        slope: float = 0.01,
        **kwargs
    ):
        super().__init__()
        self.monotonic_layer = MonotonicLayer(
            num_criteria, num_hidden_components, slope, **kwargs
        )

    def set_slope(self, val: float) -> None:
        """
        Set the slope value for the LeakyHardSigmoid activation function.

        Args:
            val (float): Slope value.
        """
        self.monotonic_layer.set_slope(val)

    def forward(self, input: torch.Tensor) -> torch.Tensor:
        """
        Forward pass of the module.

        Args:
            input (torch.Tensor): Input tensor of shape (batch_size,  num_criteria).

        Returns:
            torch.Tensor: Output tensor.
        """
        x = self.monotonic_layer(input)
        return x.sum(1)
