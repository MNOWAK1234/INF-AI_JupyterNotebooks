from __future__ import annotations

import itertools
from collections import defaultdict

import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import multivariate_normal

# UWAGA! Lektura tego pliku NIE powinna być konieczna, aby rozwiązać ćwiczenia
SUM = "suma"


def _generate(n: int, mean1: np.ndarray, mean2: np.ndarray, cov: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    x1 = np.random.multivariate_normal(mean1, cov, n)
    x2 = np.random.multivariate_normal(mean2, cov, n)
    x = np.concatenate((x1, x2))
    y = np.array([0] * n + [1] * n)
    return x, y


def generate1(n: int = 100) -> tuple[np.ndarray, np.ndarray]:
    return _generate(n, np.array([4, 4]), np.array([0, 0]), np.array([[2, 0], [0, 1]]))


def generate2(n: int = 100) -> tuple[np.ndarray, np.ndarray]:
    return _generate(n, np.array([4, 4]), np.array([0, 0]), np.array([[5, -4], [-4, 4]]))


def generate3(n: int = 100) -> tuple[np.ndarray, np.ndarray]:
    return _generate(n, np.array([4, 4]), np.array([0, 0]), np.array([[5, 4], [4, 4]]))


class FullBayes():
    def __init__(self):
        self.prob = {}
        self.class_log_prob = None

    def _hash_example(self, x: np.ndarray) -> tuple:
        return tuple(x.tolist())

    def fit(self, x: np.ndarray, y: np.ndarray) -> None:
        unique, counts = np.unique(y, return_counts=True)
        self.class_log_prob = np.log(counts / np.sum(counts))
        for i, clazz in enumerate(y):
            if clazz not in self.prob:
                if x.shape[1] > 10:
                    self.prob[clazz] = defaultdict(int)
                else:
                    self.prob[clazz] = {}
                    # Powyższe wywołanie lepiej by było zastąpić defaultdict(int)
                    # i usunięcie wpisywania zer - jednak chodzi o to by student
                    # mógł zobaczyć wszystkie estymaty (i potencjalnie sporo zer)
                    for j in itertools.product([0, 1], repeat=x.shape[1]):
                        self.prob[clazz][j] = 0
                    self.prob[clazz][SUM] = 0

            self.prob[clazz][self._hash_example(x[i])] += 1
            self.prob[clazz][SUM] += 1

    def predict_proba(self, x: np.ndarray) -> np.ndarray:
        results = np.tile(self.class_log_prob, (x.shape[0], 1))
        for i in range(x.shape[0]):
            for j in range(len(self.class_log_prob)):
                results[i, j] += np.nan_to_num(np.log(self.prob[j][self._hash_example(x[i])] / self.prob[j][SUM]))
        probabilities = np.exp(results)
        probabilities /= np.sum(probabilities, axis=1, keepdims=True)
        return probabilities

    def predict(self, x: np.ndarray) -> np.ndarray:
        prob = self.predict_proba(x)
        return np.argmax(prob, axis=1)


class SmoothFullBayes(FullBayes):
    def fit(self, x: np.ndarray, y: np.ndarray) -> None:
        unique, counts = np.unique(y, return_counts=True)
        self.class_log_prob = np.log(counts / np.sum(counts))
        for i, clazz in enumerate(y):
            if clazz not in self.prob:
                if x.shape[1] > 10:
                    self.prob[clazz] = defaultdict(lambda: 1)
                    self.prob[clazz][SUM] = 2 ** x.shape[1]
                else:
                    self.prob[clazz] = {}
                    # Powyższe wywołanie lepiej by było zastąpić defaultdict(int)
                    # i usunięcie wpisywania zer - jednak chodzi o to by student
                    # mógł zobaczyć wszystkie estymaty (i potencjalnie sporo zer)
                    for j in itertools.product([0, 1], repeat=x.shape[1]):
                        self.prob[clazz][j] = 1
                    self.prob[clazz][SUM] = len(self.prob[clazz])

            self.prob[clazz][self._hash_example(x[i])] += 1
            self.prob[clazz][SUM] += 1


class GaussianBayes():
    def __init__(self) -> None:
        self.means = {}
        self.stds = {}
        self.class_log_prob = None

    def fit(self, x: np.ndarray, y: np.ndarray) -> None:
        unique, counts = np.unique(y, return_counts=True)
        self.class_log_prob = np.log(counts / np.sum(counts))
        for clazz in unique:
            self.means[clazz] = np.mean(x[y == clazz], axis=0)
            self.stds[clazz] = np.cov(x[y == clazz].T)

    def predict_proba(self, x: np.ndarray) -> np.ndarray:
        results = np.tile(self.class_log_prob, (x.shape[0], 1))
        for i in range(len(self.class_log_prob)):
            results[:, i] += multivariate_normal.logpdf(x, self.means[i], self.stds[i])
        probabilities = np.exp(results)
        probabilities /= np.sum(probabilities, axis=1, keepdims=True)
        return probabilities

    def predict(self, x: np.ndarray) -> np.ndarray:
        prob = self.predict_proba(x)
        return np.argmax(prob, axis=1)


def plot_gaussian_bayes(x: np.ndarray, y: np.ndarray, gnb) -> None:
    plt.figure(1, figsize=(15, 5))
    plt.subplot(121)

    maxx = np.max(x, axis=0)
    minx = np.min(x, axis=0)
    eps = 0.5
    xx = np.linspace(minx[0] - eps, maxx[0] + eps, 100)
    yy = np.linspace(minx[1] - eps, maxx[1] + eps, 100).T
    xx, yy = np.meshgrid(xx, yy)
    xfull = np.c_[xx.ravel(), yy.ravel()]
    prob = gnb.predict_proba(xfull)
    _ = plt.imshow(prob[:, 0].reshape((100, 100)),
                   extent=(minx[0] - eps, maxx[0] + eps, minx[1] - eps, maxx[1] + eps), origin='lower')
    plt.xticks(())
    plt.yticks(())
    plt.scatter(x[:, 0], x[:, 1], c=y)
    plt.title('Estimated cond. probability')

    plt.subplot(122)

    colors = np.array(['red', 'blue'])
    for i in range(2):
        if len(gnb.stds[0].shape) < 2:
            z = multivariate_normal.pdf(xfull, gnb.means[i], gnb.stds[i] @ np.eye(2, 2))
        else:
            z = multivariate_normal.pdf(xfull, gnb.means[i], gnb.stds[i])
        cs = plt.contour(xx, yy, z.reshape((100, 100)), 6, colors=colors[i])
        plt.clabel(cs, fontsize=9, inline=1)
    plt.scatter(x[:, 0], x[:, 1], c=colors[y])
    plt.title('Estimated Gaussians for each class')
    plt.show()


def generate_binary(n: int, k: int = 50) -> tuple[np.ndarray, np.ndarray]:
    p = np.random.rand(k)
    x = np.random.binomial(1, p, size=(n, k))
    w = np.random.normal(0, 10, k)
    y = (1 / (1 + np.exp(-x @ w + (k / 25))) > 0.5).astype(int)
    # Nawet zbiór 2-elementowy zawiera wszystkie klasy
    y[0] = 0
    y[1] = 1
    return x, y


def plot_accuracy_iterations_plot(iterations: int, results: dict, results_train: dict) -> None:
    plt.figure(1, figsize=(15, 5))
    plt.subplot(121)
    for name, result in results.items():
        plt.plot(iterations, result, label=name)
    plt.legend()
    plt.title("Trafność na zbiorze testowym")

    plt.subplot(122)
    for name, result in results_train.items():
        plt.plot(iterations, result, label=name)
    plt.title("Trafność na zbiorze uczącym")
    plt.legend()
    plt.show()
