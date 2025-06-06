import torch
import torch.nn as nn

from threshold_layer import ThresholdLayer


class NormLayer(nn.Module):
    """
    Custom module for min-max normalization layer.

    This normalization layer performs min-max normalization on the input tensor,
    such that if all criteria are equal to 0, the output is 0, and if all criteria
    are equal to 1, the output is 1.
    Additionally, the output is thresholded using a ThresholdLayer with a threshold of 0.5.

    Args:
        method_instance (torch.nn.Module): Instance of the method to be normalized.
        num_criteria (int): Number of criteria.
    """

    def __init__(self, method_instance: torch.nn.Module, num_criteria: int):
        super().__init__()
        self.method_instance = method_instance
        self.num_criteria = num_criteria
        self.thresholdLayer = ThresholdLayer(0.5)

    def set_slope(self, slope: float):
        """
        Set the slope value for the LeakyHardSigmoid activation function.

        Args:
            val (float): Slope value.
        """
        self.method_instance.set_slope(slope)

    def forward(self, input: torch.Tensor) -> torch.Tensor:
        """
        Forward pass of the module.

        Args:
            x (torch.Tensor): Input tensor.

        Returns:
            torch.Tensor: Output tensor.
        """
        self.out = self.method_instance(input)

        zero_input = torch.zeros(self.num_criteria).view(1, 1, -1).to(self.out.device)
        zero = self.method_instance(zero_input)
        one = self.method_instance(zero_input + 1)

        self.out = (self.out - zero) / (one - zero)
        return self.thresholdLayer(self.out)
