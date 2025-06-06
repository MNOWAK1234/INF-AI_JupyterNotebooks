import torch
import torch.nn as nn


class CriterionLayerCombine(nn.Module):
    """
    Custom module to combine outputs from all hidden components.

    Args:
        num_criteria (int): Number of criteria.
        num_hidden_components (int): Number of hidden components.
        min_weight (float, optional): Minimum weight value. Defaults to 0.001.
    """

    def __init__(
        self,
        num_criteria: int,
        num_hidden_components: int,
        min_weight: float = 0.001,
        **kwargs
    ):
        super().__init__()
        self.min_weight = min_weight
        self.weight = nn.Parameter(
            torch.FloatTensor(num_hidden_components, num_criteria)
        )
        self.reset_parameters()

    def reset_parameters(self) -> None:
        """
        Reset the parameters of the module.
        """
        nn.init.uniform_(self.weight, 0.2, 1.0)
        self.weight.data = self.weight.data / torch.sum(self.weight.data)

    def compute_weight(self) -> torch.Tensor:
        """
        Compute the weight value.

        Returns:
            torch.Tensor: Weight value.
        """
        with torch.no_grad():
            self.weight.data[self.weight.data < 0] = self.min_weight
        return self.weight

    def forward(self, input: torch.Tensor) -> torch.Tensor:
        """
        Forward pass of the module.

        Args:
            input (torch.Tensor): Input tensor of shape (batch_size, num_hidden_components, num_criteria).

        Returns:
            torch.Tensor: Output tensor of shape (batch_size, num_criteria).
        """
        return (input * self.compute_weight()).sum(1)
