import torch.nn as nn

from criterion_layer_combine import CriterionLayerCombine
from criterion_layer_spread_uniform_center import CriterionLayerSpread
from leaky_hard_sigmoid import LeakyHardSigmoid


class MonotonicLayer(nn.Sequential):
    """
    Custom module for Monotonic layer.

    Args:
        num_criteria (int): Number of criteria.
        num_hidden_components (int): Number of hidden units.
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
        self.criterion_layer_spread = CriterionLayerSpread(
            num_criteria, num_hidden_components, **kwargs
        )
        self.activation_function = LeakyHardSigmoid(slope=slope, **kwargs)
        self.criterion_layer_combine = CriterionLayerCombine(
            num_criteria, num_hidden_components, **kwargs
        )

    def set_slope(self, val: float) -> None:
        """
        Set the slope value for the LeakyHardSigmoid activation function.

        Args:
            val (float): Slope value.
        """
        self.activation_function.set_slope(val)

    def forward(self, x):
        """
        Forward pass of the module.

        Args:
            x (torch.Tensor): Input tensor of shape (batch_size, num_criteria).

        Returns:
            torch.Tensor: Output tensor of shape (batch_size, num_criteria).
        """
        for module in self._modules.values():
            x = module(x)
        return x
