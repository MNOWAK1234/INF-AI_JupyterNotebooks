from pathlib import Path

import click
import numpy as np
import pandas as pd

from promethee_pl.utils import (
    load_dataset,
    load_preference_information,
    display_ranking,
)


def calculate_marginal_preference_matrix(
    dataset: pd.DataFrame, preference_information: pd.DataFrame
) -> np.ndarray:
    """
    Function that calculates the marginal preference matrix all alternatives pairs and criterion available in dataset

    :param dataset: difference between compared alternatives
    :param preference_information: preference information
    :return: 3D numpy array with marginal preference matrix on every parser, Consecutive indices [i, j, k] describe first alternative, second alternative, parser
    """
    n_alternatives = dataset.shape[0]
    n_criteria = preference_information.shape[0]

    marginal_preference_matrix = np.zeros((n_alternatives, n_alternatives, n_criteria))

    for k, (criterion, row) in enumerate(preference_information.iterrows()):
        p = row["p"]
        q = row["q"]
        for i, a in enumerate(dataset[criterion]):
            for j, b in enumerate(dataset[criterion]):
                if i == j:
                    marginal_preference_matrix[i][j][k] = 0
                    continue
                if row["type"] == "gain":
                    if a - b >= p:
                        marginal_preference_matrix[i][j][k] = 1
                    elif a - b <= q:
                        marginal_preference_matrix[i][j][k] = 0
                    else:
                        marginal_preference_matrix[i][j][k] = (a-b-q)/(p-q)
                else:
                    if b - a >= p:
                        marginal_preference_matrix[i][j][k] = 1
                    elif b - a <= q:
                        marginal_preference_matrix[i][j][k] = 0
                    else:
                        marginal_preference_matrix[i][j][k] = (b-a-q)/(p-q)

    return marginal_preference_matrix


def calculate_comprehensive_preference_index(
    marginal_preference_matrix: np.ndarray, preference_information: pd.DataFrame
) -> np.ndarray:
    """
    Function that calculates comprehensive preference index for the given dataset

    :param marginal_preference_matrix: 3D numpy array with marginal preference matrix on every parser, Consecutive indices [i, j, k] describe first alternative, second alternative, criterion
    :param preference_information: Padnas dataframe containing preference information
    :return: 2D numpy array with marginal preference matrix. Every entry in the matrix [i, j] represents comprehensive preference index between alternative i and alternative j
    """
    n_alternatives = marginal_preference_matrix.shape[0]

    comprehensive_preference_matrix = np.zeros((n_alternatives, n_alternatives))

    for i in range(n_alternatives):
        for j in range(n_alternatives):
            sum_values = 0
            sum_weights = 0
            for k, (_, row) in enumerate(preference_information.iterrows()):
                weight = row["k"]
                sum_values += marginal_preference_matrix[i][j][k] * weight
                sum_weights += weight
            comprehensive_preference_matrix[i][j] = sum_values / sum_weights

    return comprehensive_preference_matrix


def calculate_positive_flow(
    comprehensive_preference_matrix: np.ndarray, alternatives: pd.Index
) -> pd.Series:
    """
    Function that calculates the positive flow value for the given preference matrix and corresponding index

    :param comprehensive_preference_matrix: 2D numpy array with marginal preference matrix. Every entry in the matrix [i, j] represents comprehensive preference index between alternative i and alternative j
    :param alternatives: index representing the alternative name in the corresponding position in preference matrix
    :return: series representing positive flow values for the given preference matrix
    """
    flow = {}
    for i, alternative in enumerate(alternatives):
        sum_row = 0
        for j in range(comprehensive_preference_matrix.shape[1]):
            sum_row += comprehensive_preference_matrix[i][j]
        flow[alternative] = sum_row

    return pd.Series(data=flow, index=alternatives)


def calculate_negative_flow(
    comprehensive_preference_matrix: np.ndarray, alternatives: pd.Index
) -> pd.Series:
    """
    Function that calculates the negative flow value for the given preference matrix and corresponding index

    :param comprehensive_preference_matrix: 2D numpy array with marginal preference matrix. Every entry in the matrix [i, j] represents comprehensive preference index between alternative i and alternative j
    :param alternatives: index representing the alternative name in the corresponding position in preference matrix
    :return: series representing negative flow values for the given preference matrix
    """
    flow = {}
    for i, alternative in enumerate(alternatives):
        sum_column = 0
        for j in range(comprehensive_preference_matrix.shape[0]):
            sum_column += comprehensive_preference_matrix[j][i]
        flow[alternative] = sum_column

    return pd.Series(data=flow, index=alternatives)


def calculate_net_flow(positive_flow: pd.Series, negative_flow: pd.Series) -> pd.Series:
    """
    Function that calculates the net flow value for the given positive and negative flow

    :param positive_flow: series representing positive flow values for the given preference matrix
    :param negative_flow: series representing negative flow values for the given preference matrix
    :return: series representing net flow values for the given preference matrix
    """
    flow = {}
    for (alternative, positive), (_, negative) in zip(positive_flow.items(), negative_flow.items()):
        flow[alternative] = positive - negative
    return pd.Series(data=flow, index=positive_flow.index)


def create_partial_ranking(
    positive_flow: pd.Series, negative_flow: pd.Series
) -> pd.DataFrame:
    """
    Function that aggregates positive and negative flow to a partial ranking (from Promethee I)

    :param positive_flow: series representing positive flow values for the given preference matrix
    :param negative_flow: series representing negative flow values for the given preference matrix
    :return: partial ranking in a form of outranking matrix, as Dataframe where in index and columns are alternatives, i.e.
    1- if for the give pair [i, j] the alternative i is preferred over j or i is indifferent from j
    0- otherwise
    """
    outranking_matrix = np.zeros((positive_flow.size, positive_flow.size))
    for i, ((_, positive_i), (_, negative_i)) in enumerate(zip(positive_flow.items(), negative_flow.items())):
        for j, ((_, positive_j), (_, negative_j)) in enumerate(zip(positive_flow.items(), negative_flow.items())):
            # check indifference
            if positive_i == positive_j and negative_i == negative_j:
                outranking_matrix[i][j] = 0
            # check preference
            elif positive_i >= positive_j and negative_i <= negative_j:
                outranking_matrix[i][j] = 1
        
    outranking_matrix = np.array(outranking_matrix, dtype='bool')
    return pd.DataFrame(outranking_matrix, columns=positive_flow.index, index=positive_flow.index)


def create_complete_ranking(net_flow: pd.Series) -> pd.DataFrame:
    """
    Function that aggregates positive and negative flow to a complete ranking (from Promethee II)
    :param net_flow: series representing net flow values for the given preference matrix
    :return: complete ranking in a form of outranking matrix, as Dataframe where in index and columns are alternatives, i.e.
    1- if for the give pair [i, j] the alternative i is preferred over j or i is indifferent from j
    0- otherwise
    """
    outranking_matrix = np.zeros((net_flow.size, net_flow.size))
    for i, (_, value_i) in enumerate(net_flow.items()):
        for j, (_, value_j) in enumerate(net_flow.items()):
            if value_i >= value_j:
                outranking_matrix[i][j] = 1
    
    outranking_matrix = np.array(outranking_matrix, dtype='bool')
    return pd.DataFrame(outranking_matrix, columns=net_flow.index, index=net_flow.index)


@click.command()
@click.argument("dataset_path", type=click.Path(exists=True))
def main(dataset_path: str) -> None:
    dataset_path = Path(dataset_path)

    dataset = load_dataset(dataset_path)
    preference_information = load_preference_information(dataset_path)

    marginal_preference_matrix = calculate_marginal_preference_matrix(
        dataset, preference_information
    )
    comprehensive_preference_matrix = calculate_comprehensive_preference_index(
        marginal_preference_matrix, preference_information
    )

    positive_flow = calculate_positive_flow(
        comprehensive_preference_matrix, dataset.index
    )
    negative_flow = calculate_negative_flow(
        comprehensive_preference_matrix, dataset.index
    )

    assert positive_flow.index.equals(negative_flow.index)

    partial_ranking = create_partial_ranking(positive_flow, negative_flow)
    display_ranking(partial_ranking, "Promethee I")

    net_flow = calculate_net_flow(positive_flow, negative_flow)
    complete_ranking = create_complete_ranking(net_flow)
    display_ranking(complete_ranking, "Promethee II")


if __name__ == "__main__":
    main()
