from pathlib import Path

import click
import numpy as np
import pandas as pd

from electre_iii_pl.utils import (
    load_dataset,
    load_preference_information,
    display_ranking,
)


def calculate_marginal_concordance_matrix(
    dataset: pd.DataFrame, preference_information: pd.DataFrame
) -> np.ndarray:
    """
    Function that calculates the marginal concordance matrix for all alternatives pairs and criterion available in dataset

    :param dataset: pandas dataframe representing dataset with alternatives as rows and criterion as columns
    :param preference_information: pandas dataframe with preference information
    :return: 3D numpy array with marginal concordance matrix with shape [number of alternatives, number of alternatives, number of criterion], where element with index [i, j, k] describe marginal concordance index between alternative i and alternative j on criterion k
    """
    n_alternatives = dataset.shape[0]
    n_criteria = preference_information.shape[0]

    marginal_concordance_matrix = np.zeros((n_alternatives, n_alternatives, n_criteria))

    for k, (criterion, row) in enumerate(preference_information.iterrows()):
        p = row["p"]
        q = row["q"]
        for i, a in enumerate(dataset[criterion]):
            for j, b in enumerate(dataset[criterion]):
                if row["type"] == "gain":
                    if b - a <= q:
                        marginal_concordance_matrix[i][j][k] = 1
                    elif b - a >= p:
                        marginal_concordance_matrix[i][j][k] = 0
                    else:
                        marginal_concordance_matrix[i][j][k] = (b-a-q)/(p-q)
                else:
                    if a - b <= q:
                        marginal_concordance_matrix[i][j][k] = 1
                    elif a - b >= p:
                        marginal_concordance_matrix[i][j][k] = 0
                    else:
                        marginal_concordance_matrix[i][j][k] = (a-b-q)/(p-q)

    return marginal_concordance_matrix


def calculate_comprehensive_concordance_matrix(
    marginal_concordance_matrix: np.ndarray, preference_information: pd.DataFrame
) -> np.ndarray:
    """
    Function that calculates comprehensive concordance matrix for the given dataset

    :param marginal_concordance_matrix: 3D numpy array with marginal concordance matrix with shape [number of alternatives, number of alternatives, number of criterion], where element with index [i, j, k] describe marginal concordance index between alternative i and alternative j on criterion k
    :param preference_information: pandas dataframe with preference information
    :return: 2D numpy array with comprehensive concordance matrix with shape [number of alternatives, number of alternatives], where element with index [i, j] describe comprehensive concordance index between alternative i and alternative j
    """
    n_alternatives = marginal_concordance_matrix.shape[0]
    comprehensive_concordance_matrix = np.zeros((n_alternatives, n_alternatives))

    for i in range(n_alternatives):
        for j in range(n_alternatives):
            sum_values = 0
            sum_weights = 0
            for k, (_, row) in enumerate(preference_information.iterrows()):
                weight = row["k"]
                sum_values += marginal_concordance_matrix[i][j][k] * weight
                sum_weights += weight
            comprehensive_concordance_matrix[i][j] = sum_values / sum_weights

    return comprehensive_concordance_matrix


def calculate_marginal_discordance_matrix(
    dataset: pd.DataFrame, preference_information: pd.DataFrame
) -> np.ndarray:
    """
    Function that calculates the marginal discordance matrix for all alternatives pairs and criterion available in dataset

    :param dataset: pandas dataframe representing dataset with alternatives as rows and criterion as columns
    :param preference_information: pandas dataframe with preference information
    :return: 3D numpy array with marginal discordance matrix with shape [number of alternatives, number of alternatives, number of criterion], where element with index [i, j, k] describe marginal discordance index between alternative i and alternative j on criterion k
    """
    n_alternatives = dataset.shape[0]
    n_criteria = preference_information.shape[0]

    marginal_discordance_matrix = np.zeros((n_alternatives, n_alternatives, n_criteria))

    for k, (criterion, row) in enumerate(preference_information.iterrows()):
        p = row["p"]
        v = row["v"]
        for i, a in enumerate(dataset[criterion]):
            for j, b in enumerate(dataset[criterion]):
                if i == j:
                    marginal_discordance_matrix[i][j][k] = 0
                    continue
                if row["type"] == "gain":
                    if b - a >= v:
                        marginal_discordance_matrix[i][j][k] = 1
                    elif b - a <= p:
                        marginal_discordance_matrix[i][j][k] = 0
                    else:
                        marginal_discordance_matrix[i][j][k] = (b-a-p)/(v-p)
                else:
                    if a - b >= v:
                        marginal_discordance_matrix[i][j][k] = 1
                    elif a - b <= p:
                        marginal_discordance_matrix[i][j][k] = 0
                    else:
                        marginal_discordance_matrix[i][j][k] = (a-b-p)/(v-p)

    return marginal_discordance_matrix


def calculate_credibility_index(
    comprehensive_concordance_matrix: np.ndarray,
    marginal_discordance_matrix: np.ndarray,
) -> np.ndarray:
    """
    Function that calculates the credibility index for the given comprehensive concordance matrix and marginal discordance matrix

    :param comprehensive_concordance_matrix: 2D numpy array with comprehensive concordance matrix. Every entry in the matrix [i, j] represents comprehensive concordance index between alternative i and alternative j
    :param marginal_discordance_matrix: 2D numpy array with marginal discordance matrix, Consecutive indices [i, j, k] describe first alternative, second alternative, criterion
    :return: 2D numpy array with credibility matrix with shape [number of alternatives, number of alternatives], where element with index [i, j] describe credibility index between alternative i and alternative j
    """
    credibility_matrix = np.zeros_like(comprehensive_concordance_matrix)

    for i in range(comprehensive_concordance_matrix.shape[0]):
        for j in range(comprehensive_concordance_matrix.shape[1]):
            credibility_matrix[i][j] = comprehensive_concordance_matrix[i][j]
            for k in range(marginal_discordance_matrix.shape[2]):
                if marginal_discordance_matrix[i][j][k] > comprehensive_concordance_matrix[i][j]:
                    credibility_matrix[i][j] *= (1 - marginal_discordance_matrix[i][j][k]) / (1 - comprehensive_concordance_matrix[i][j])

    return credibility_matrix


def distillation(
    credibility_index: np.ndarray,
    alternatives: pd.Index,
    alpha: float = -0.15,
    beta: float = 0.3,
    distillation_mode: str = "descending",
) -> tuple[list, bool]:
    delta = 0
    for i in range(len(alternatives)):
        for j in range(len(alternatives)):
            if i != j and credibility_index[i][j] > delta:
                delta = credibility_index[i][j]

    if delta == 0:
        return [i for i in range(alternatives.size)], True

    s = alpha * delta + beta
    
    next_delta = 0
    for i in range(len(alternatives)):
        for j in range(len(alternatives)):
            if i != j and credibility_index[i][j] > next_delta and credibility_index[i][j] < delta - s:
                next_delta = credibility_index[i][j]

    delta = next_delta
    non_zero_matrix = np.zeros_like(credibility_index)
    for i in range(len(alternatives)):
        for j in range(len(alternatives)):
            if i != j and credibility_index[i][j] > delta:
                if credibility_index[i][j] > credibility_index[j][i] + alpha * credibility_index[i][j] + beta:
                    non_zero_matrix[i][j] = 1

    quality = np.zeros((len(alternatives)))
    for i in range(len(alternatives)):
        quality[i] = np.sum(non_zero_matrix, axis=1)[i] - np.sum(non_zero_matrix, axis=0)[i]

    if distillation_mode == "descending":
        return [i for i, value in enumerate(quality) if value == max(quality)], False
    else:
        return [i for i, value in enumerate(quality) if value == min(quality)], False


def descending_distillation(
    credibility_index: np.ndarray,
    alternatives: pd.Index,
    alpha: float = -0.15,
    beta: float = 0.3,
) -> pd.DataFrame:
    """
    Function that calculates the descending distillation procedure

    :param credibility_index: 2D numpy array with credibility matrix with shape [number of alternatives, number of alternatives], where element with index [i, j] describe credibility index between alternative i and alternative j
    :param alternatives: index representing the alternative name in the corresponding position in preference matrix
    :param alpha: the parameter alpha for the s function
    :param beta: the parameter beta for the s function
    :return: descending ranking in a form of outranking matrix, as Dataframe where in index and columns are alternatives, i.e.
    1- if for the give pair [i, j] the alternative i is preferred over j or i is indifferent from j
    0- otherwise
    """
    outranking_matrix = np.zeros_like(credibility_index)

    # -1 denotes that the value has not yet been decided
    outranking_matrix.fill(-1)

    # aPa for each alternative a
    np.fill_diagonal(outranking_matrix, 1)

    all_alternatives = alternatives

    while True:
        best, done = distillation(credibility_index, alternatives, alpha, beta, "descending")
        if done:
            for i in range(outranking_matrix.shape[0]):
                for j in range(outranking_matrix.shape[1]):
                    if outranking_matrix[i][j] == -1:
                        outranking_matrix[i][j] = 1
            break

        # add the best alternative to the ranking and remove from alternatives
        if len(best) == 1:
            alternative = alternatives[best[0]]
            index = all_alternatives.get_loc(alternative)

            credibility_index = np.delete(credibility_index, best[0], 0)
            credibility_index = np.delete(credibility_index, best[0], 1)
            alternatives = alternatives.delete(best[0])

            for i in range(outranking_matrix.shape[0]):
                if outranking_matrix[index][i] == -1:
                    outranking_matrix[index][i] = 1
            
            for i in range(outranking_matrix.shape[1]):
                if outranking_matrix[i][index] == -1:
                    outranking_matrix[i][index] = 0

            continue

        # inner distillation
        inner_matrix = credibility_index[best,:][:,best]
        inner_alternatives = alternatives[best]

        best, done = distillation(inner_matrix, inner_alternatives, alpha, beta, "descending")

        # find original indices of the chosen alternatives
        indices = []
        for i in best:
            alternative = inner_alternatives[i]
            index = alternatives.get_loc(alternative)
            indices.append(index)
            
        # remove alternatives
        credibility_index = np.delete(credibility_index, indices, 0)
        credibility_index = np.delete(credibility_index, indices, 1)
        alternatives = alternatives.delete(indices)

        # check if array is empty
        if len(alternatives) == 0:
            for i in range(outranking_matrix.shape[0]):
                for j in range(outranking_matrix.shape[1]):
                    if outranking_matrix[i][j] == -1:
                        outranking_matrix[i][j] = 1
            break

        for i in best:
            for j in range(outranking_matrix.shape[0]):
                if outranking_matrix[index][j] == -1:
                    outranking_matrix[index][j] = 1
            
            for j in range(outranking_matrix.shape[1]):
                if outranking_matrix[j][index] == -1:
                    outranking_matrix[j][index] = 0


    outranking_matrix = np.array(outranking_matrix, dtype='bool')
    return pd.DataFrame(outranking_matrix, columns=all_alternatives, index=all_alternatives)


def ascending_distillation(
    credibility_index: np.ndarray,
    alternatives: pd.Index,
    alpha: float = -0.15,
    beta: float = 0.3,
) -> pd.DataFrame:
    """
    Function that calculates the ascending distillation procedure

    :param credibility_index: 2D numpy array with credibility matrix with shape [number of alternatives, number of alternatives], where element with index [i, j] describe credibility index between alternative i and alternative j
    :param alternatives: index representing the alternative name in the corresponding position in preference matrix
    :param alpha: the parameter alpha for the s function
    :param beta: the parameter beta for the s function
    :return: ascending ranking in a form of outranking matrix, as Dataframe where in index and columns are alternatives, i.e.
    1- if for the give pair [i, j] the alternative i is preferred over j or i is indifferent from j
    0- otherwise
    """
    outranking_matrix = np.zeros_like(credibility_index)

    # -1 denotes that the value has not yet been decided
    outranking_matrix.fill(-1)

    # aPa for each alternative a
    np.fill_diagonal(outranking_matrix, 1)

    all_alternatives = alternatives

    while True:
        worst, done = distillation(credibility_index, alternatives, alpha, beta, "ascending")
        if done:
            for i in range(outranking_matrix.shape[0]):
                for j in range(outranking_matrix.shape[1]):
                    if outranking_matrix[i][j] == -1:
                        outranking_matrix[i][j] = 1
            break

        # add the worst alternative to the ranking and remove from alternatives
        if len(worst) == 1:
            alternative = alternatives[worst[0]]
            index = all_alternatives.get_loc(alternative)

            credibility_index = np.delete(credibility_index, worst[0], 0)
            credibility_index = np.delete(credibility_index, worst[0], 1)
            alternatives = alternatives.delete(worst[0])

            for i in range(outranking_matrix.shape[0]):
                if outranking_matrix[index][i] == -1:
                    outranking_matrix[index][i] = 0
            
            for i in range(outranking_matrix.shape[1]):
                if outranking_matrix[i][index] == -1:
                    outranking_matrix[i][index] = 1

            continue

        # inner distillation
        inner_matrix = credibility_index[worst,:][:,worst]
        inner_alternatives = alternatives[worst]

        worst, done = distillation(inner_matrix, inner_alternatives, alpha, beta, "ascending")

        # find original indices of the chosen alternatives
        indices = []
        for i in worst:
            alternative = inner_alternatives[i]
            index = alternatives.get_loc(alternative)
            indices.append(index)
            
        # remove alternatives
        credibility_index = np.delete(credibility_index, indices, 0)
        credibility_index = np.delete(credibility_index, indices, 1)
        alternatives = alternatives.delete(indices)

        # check if array is empty
        if len(alternatives) == 0:
            for i in range(outranking_matrix.shape[0]):
                for j in range(outranking_matrix.shape[1]):
                    if outranking_matrix[i][j] == -1:
                        outranking_matrix[i][j] = 1
            break

        for i in worst:
            for j in range(outranking_matrix.shape[0]):
                if outranking_matrix[index][j] == -1:
                    outranking_matrix[index][j] = 0
            
            for j in range(outranking_matrix.shape[1]):
                if outranking_matrix[j][index] == -1:
                    outranking_matrix[j][index] = 1


    outranking_matrix = np.array(outranking_matrix, dtype='bool')
    return pd.DataFrame(outranking_matrix, columns=all_alternatives, index=all_alternatives)          


def create_final_ranking(
    descending_ranking: pd.DataFrame, ascending_ranking: pd.DataFrame
) -> pd.DataFrame:
    """
    Function that computes the final ranking from both ascending and descending ranking

    :param descending_ranking: dataframe representing descending ranking
    :param ascending_ranking: dataframe representing ascending ranking
    :return: final ranking in a form of outranking matrix, as Dataframe where in index and columns are alternatives, i.e.
    1- if for the give pair [i, j] the alternative i is preferred over j or i is indifferent from j
    0- otherwise
    """
    return descending_ranking & ascending_ranking


@click.command()
@click.argument("dataset_path", type=click.Path(exists=True))
def main(dataset_path: str) -> None:
    dataset_path = Path(dataset_path)

    dataset = load_dataset(dataset_path)
    preference_information = load_preference_information(dataset_path)

    marginal_concordance_matrix = calculate_marginal_concordance_matrix(
        dataset, preference_information
    )
    comprehensive_concordance_matrix = calculate_comprehensive_concordance_matrix(
        marginal_concordance_matrix, preference_information
    )
    marginal_discordance_matrix = calculate_marginal_discordance_matrix(
        dataset, preference_information
    )
    credibility_index = calculate_credibility_index(
        comprehensive_concordance_matrix, marginal_discordance_matrix
    )

    descending_ranking = descending_distillation(credibility_index, dataset.index)
    display_ranking(descending_ranking, "Descending Ranking")

    ascending_ranking = ascending_distillation(credibility_index, dataset.index)
    display_ranking(ascending_ranking, "Ascending Ranking")

    final_ranking = create_final_ranking(descending_ranking, ascending_ranking)
    display_ranking(final_ranking, "Final Ranking")


if __name__ == "__main__":
    main()
