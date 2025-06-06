import torch.nn as nn
import torch.nn.functional as F


class LeakyHardSigmoid(nn.Module):
    """
    Custom module for LeakyHardSigmoid activation function.

    Args:
        slope (float, optional): Slope value for function. Defaults to 0.01.
    """

    def __init__(self, slope: float = 0.01, **kwargs):
        super().__init__()
        self.slope = slope

    def set_slope(self, val: float) -> None:
        """
        Set the slope value for the function.

        Args:
            val (float): Slope value.
        """
        self.slope = val

    def forward(self, input):
        """
        Forward pass of the module.

        Args:
            input (torch.Tensor): Input tensor.

        Returns:
            torch.Tensor: Output tensor.
        """
        return F.leaky_relu(1.0 - F.leaky_relu(1 - input, self.slope), self.slope)
