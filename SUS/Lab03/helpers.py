import matplotlib.pyplot as plt
import numpy as np


def plot_classification(x: np.ndarray, y: np.ndarray, labels: np.ndarray, a=None, b=None, c=None):
    if None not in [a, b, c]:
        delta = 0.025
        x_vals = np.arange(min(x), max(x), delta)
        y_vals = np.arange(min(y), max(y), delta)
        x_mesh, y_mesh = np.meshgrid(x_vals, y_vals)
        z = a * x_mesh + b * y_mesh + c
        cs = plt.contour(x_mesh, y_mesh, z, 6, colors='k')
        plt.clabel(cs, fontsize=9, inline=1)
    plt.scatter(x.ravel(), y.ravel(), c=labels, cmap='bwr')  # Flatten x and y
    plt.show()


def get_separable(sklearn: bool = False
                  ) -> tuple[np.ndarray, np.ndarray] | tuple[np.ndarray, np.ndarray, np.ndarray]:
    dane = np.load('data_svm.npz')
    x = dane['x']
    y = dane['y']
    labels = dane['label']
    if sklearn:
        return np.concatenate([x.reshape(-1, 1), y.reshape(-1, 1)], axis=1), labels
    else:
        return x, y, labels


def get_non_separable(sklearn: bool = False, n: int = 100
                      ) -> tuple[np.ndarray, np.ndarray] | tuple[np.ndarray, np.ndarray, np.ndarray]:
    x1 = np.random.multivariate_normal(np.array([4, 4]), np.array([[2, 0], [0, 1]]), n)
    x2 = np.random.multivariate_normal(np.array([0, 0]), np.array([[2, 0], [0, 1]]), n)
    x = np.concatenate((x1, x2))
    y = np.array([-1] * n + [1] * n)
    if sklearn:
        return x, y
    else:
        return x[:, 0], x[:, 1], y


def plot_classifier(x: np.ndarray, y: np.ndarray, gnb):
    plt.figure(1, figsize=(15, 5))
    max_ = np.max(x, axis=0)
    min_ = np.min(x, axis=0)
    eps = 0.5
    xx = np.linspace(min_[0] - eps, max_[0] + eps, 100)
    yy = np.linspace(min_[1] - eps, max_[1] + eps, 100).T
    xx, yy = np.meshgrid(xx, yy)
    xfull = np.c_[xx.ravel(), yy.ravel()]
    prob = gnb.predict(xfull).reshape(xx.shape)
    plt.contourf(xx, yy, prob, cmap=plt.cm.brg, alpha=0.2)
    plt.xticks(())
    plt.yticks(())
    plt.scatter(x[:, 0], x[:, 1], c=y)
    plt.title('Granica decyzji')


def get_wine() -> tuple[np.ndarray, np.ndarray]:
    import pandas as pd
    df_r = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine-quality/winequality-red.csv', sep=";",
                       na_values="?")
    df_r.dropna(inplace=True)
    df_w = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine-quality/winequality-white.csv', sep=";",
                       na_values="?")
    df_w.dropna(inplace=True)
    x = pd.concat([df_r, df_w]).values
    y = np.concatenate([np.zeros(len(df_r)), np.ones(len(df_w))])
    return x, y
