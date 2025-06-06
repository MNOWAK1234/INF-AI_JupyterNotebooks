from typing import Tuple

import torch
import torch.nn as nn


class CriterionLayerSpread(nn.Module):
    """
    Custom module for spreading the input value to all hidden components.

    Args:
        num_criteria (int): Number of criteria.
        num_hidden_components (int): Number of hidden components.
        input_range (Tuple[float, float], optional): Range of input values. Defaults to (0, 1).
        normalize_bias (bool, optional): Flag to normalize the bias. Defaults to False.
    """

    def __init__(
        self,
        num_criteria: int,
        num_hidden_components: int,
        input_range: Tuple[float, float] = (0, 1),
        normalize_bias: bool = False,
        **kwargs
    ) -> None:
        super().__init__()
        self.num_criteria = num_criteria
        input_range = (-input_range[0], -input_range[1])
        self.max_bias = max(input_range)
        self.min_bias = min(input_range)
        self.normalize_bias = normalize_bias
        self.bias = nn.Parameter(torch.FloatTensor(num_hidden_components, num_criteria))
        self.weight = nn.Parameter(
            torch.FloatTensor(num_hidden_components, num_criteria)
        )
        self.reset_parameters()
        self.min_w = 0

    def reset_parameters(self) -> None:
        """
        Reset the parameters of the module.
        """
        nn.init.uniform_(self.weight, 1, 10.0)
        nn.init.uniform_(self.bias, self.min_bias, self.max_bias)

    def compute_bias(self) -> torch.Tensor:
        """
        Compute the bias value.

        Returns:
            torch.Tensor: Bias value.
        """
        if self.normalize_bias:
            return torch.clamp(self.bias, self.min_bias, self.max_bias)
        else:
            return self.bias

    def compute_weight(self) -> torch.Tensor:
        """
        Compute the weight value.

        Returns:
            torch.Tensor: Weight value.
        """
        # return torch.clamp(self.weight, 0.0)
        with torch.no_grad():
            self.weight.data[self.weight.data < 0] = self.min_w
        return self.weight

    def forward(self, input: torch.Tensor) -> torch.Tensor:
        """
        Forward pass of the module.

        Args:
            input (torch.Tensor): Input tensor of shape (batch_size, num_criteria).

        Returns:
            torch.Tensor: Output tensor of shape (batch_size, num_hidden_components, num_criteria).
        """
        x = input.view(-1, 1, self.num_criteria)
        return (x + self.compute_bias()) * self.compute_weight()
