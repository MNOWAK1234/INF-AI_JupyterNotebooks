import numpy as np
import matplotlib.pyplot as plt
from math import exp, sin, ceil, cos, log
from autograd import grad
import autograd.numpy as anp

plt.style.use("ggplot")


def _annotate_points_with_numbers(x, y):
    ax = plt.gca()
    y_span = max(y) - min(y)
    c = 0.05
    for i in range(len(x)):
        ax.text(x[i], y[i] + c * y_span, str(i), ha="center", va="center", size=8)


class Function(object):
    def __init__(self, func, domain=(-10, 10), name="???"):
        self.name = name
        self.domain = domain
        self._func = func
        self._grad = grad(self._func)

    # Returns value of the function at point x
    def evaluate(self, x):
        return self._func(x)

    # Returns value of the first derivative of function at point x
    def evaluate_derivative(self, x):
        return self._grad(float(x))

    # Plots the function, points argument places additional points (e.g. steps of your optimization)
    def plot(self, domain=None, resolution=200,
             annotate_head=10, annotate_tail=10,
             points=None, minimizer=None):
        plt.clf()
        if domain is None:
            a, b = self.domain
        else:
            a, b = domain
        x = np.linspace(a, b, resolution)
        y = [self._func(xi) for xi in x]

        plt.plot(x, y)

        if points is not None:
            x = points
            y = [self._func(xi) for xi in x]

            plt.scatter(x, y, s=10)

            if len(x) > annotate_head + annotate_tail:
                x = list(x[0:annotate_head]) + list(x[-annotate_tail:])
                y = list(y[0:annotate_head]) + list(y[-annotate_tail:])

            _annotate_points_with_numbers(x, y)
        if minimizer is not None:
            plt.scatter([minimizer], [self._func(minimizer)], s=150, marker="*")
        if self.name is not None:
            plt.title(self.name)
        plt.show()


functions = [
    Function(lambda x: x ** 2, name="x^2", domain=[-10,8]),
    Function(lambda x: x ** 3, name="x^3", domain=[-5,50]),
    Function(lambda x: anp.exp((x ** 2)), name="exp(x^2)", domain=[-1, 1.2]),
    Function(lambda x: anp.log(x) ** 2 / x, name="log(x)^2/x", domain=[0.4, 12]),
    Function(lambda x: x ** 3 - 3 * x + 9, name="x^3-3x+9", domain=[-3, 2]),
    Function(lambda x: 2 * (x - 2) * (x - 3), name="2(x-2)(x-3)"),
    Function(lambda x: -x, name="-x"),
    Function(lambda x: abs(x), name="abs"),
    Function(lambda x: -anp.exp(-x ** 2), name="sort-of-gauss",domain=[-5,3]),
    Function(lambda x: anp.sin(x), name="sine wave"),
    Function(lambda x: 0.05 * x ** 2 + anp.sin(3 * x), domain=[- 12,8],name='sine wave with a "twist"'),
    Function(lambda x: anp.ceil(abs(x)),domain=[- 12,8.2], name="steps"),
]
