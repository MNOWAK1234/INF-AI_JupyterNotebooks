import numpy as np
from numpy import asarray
from autograd import grad, jacobian, hessian
from autograd.numpy import sum, prod, exp, cos, sin, sqrt, arange, pi, abs, log


class BenchmarkFunction:
    """
    Defines a global optimization benchmark problem.

    This abstract class defines the basic structure of a global
    optimization problem. Subclasses should implement the ``fun`` method
    for a particular optimization problem.

    Attributes
    ----------
    N : int
        The dimensionality of the problem.
    bounds : sequence
        The lower/upper bounds to be used for minimizing the problem.
        This a list of (lower, upper) tuples that contain the lower and upper
        bounds for the problem.  The problem should not be asked for evaluation
        outside these bounds. ``len(bounds) == N``.
    xmin : sequence
        The lower bounds for the problem
    xmax : sequence
        The upper bounds for the problem
    fglob : float
        The global minimum of the evaluated function.
    global_optimum : sequence
        A list of vectors that provide the locations of the global minimum.
        Note that some problems have multiple global minima, not all of which
        may be listed.
    nfev : int
        the number of function evaluations that the object has been asked to
        calculate.
    ngrad : int
        the number of gradient evaluations that the object has been asked to
        calculate.
    nhess : int
        the number of hessian evaluations that the object has been asked to
        calculate.
    change_dimensionality : bool
        Whether we can change the benchmark function `x` variable length (i.e.,
        the dimensionality of the problem)
    custom_bounds : sequence
        a list of tuples that contain lower/upper bounds for use in plotting.
    """

    def __init__(self, budget, dimensions):
        """
        Initialises the problem

        Parameters
        ----------

        budget : int
            The number of allowed function/gradient/hessian evaluations

        dimensions : int
            The dimensionality of the problem
        """

        self._budget = budget
        self._dimensions = dimensions
        self.nfev = 0
        self.ngrad = 0
        self.nhess = 0
        self.fglob = np.nan
        self.global_optimum = None
        self.change_dimensionality = False
        self.custom_bounds = None

        self._grad_func = grad(lambda x: self._func(x))
        # self._hess_func = jacobian(self._grad_func)
        self._hess_func = hessian(lambda x: self._func(x))

    def __str__(self):
        return f"{self.__class__.__name__} ({self.N} dimensions)"

    def __repr__(self):
        return self.__class__.__name__

    def initial_vector(self):
        """
        Random initialisation for the benchmark problem.

        Returns
        -------
        x : sequence
            a vector of length ``N`` that contains random floating point
            numbers that lie between the lower and upper bounds for a given
            parameter.
        """

        return asarray([np.random.uniform(l, u) for l, u in self.bounds])

    def success(self, x, tol=1.0e-5):
        """
        Tests if a candidate solution at the global minimum.
        The default test is

        Parameters
        ----------
        x : sequence
            The candidate vector for testing if the global minimum has been
            reached. Must have ``len(x) == self.N``
        tol : float
            The evaluated function and known global minimum must differ by less
            than this amount to be at a global minimum.

        Returns
        -------
        bool : is the candidate vector at the global minimum?
        """
        val = self._func(asarray(x))
        if abs(val - self.fglob) < tol:
            return True

        # the solution should still be in bounds, otherwise immediate fail.
        bounds = np.asarray(self.bounds, dtype=np.float64)
        if np.any(x > bounds[:, 1]):
            return False
        if np.any(x < bounds[:, 0]):
            return False

        # you found a lower global minimum.  This shouldn't happen.
        if val < self.fglob:
            raise ValueError("Found a lower global minimum", x, val, self.fglob)

        return False

    def _update_check_budget(self):
        if self._budget is not None:
            if self._budget <= 0:
                raise ValueError("Budget exceeded")
            self._budget -= 1

    def _check_bounds(self, x):
        bounds = np.asarray(self.bounds, dtype=np.float64)
        if np.any(x > bounds[:, 1]):
            raise ValueError("x is outside of bounds")
        if np.any(x < bounds[:, 0]):
            raise ValueError("x is outside of bounds")

    def _func(self, x):
        raise NotImplementedError

    def func(self, x):
        """
        Evaluation of the benchmark function at point x.

        Parameters
        ----------
        x : sequence
            The candidate vector for evaluating the benchmark problem. Must
            have ``len(x) == self.N``.

        Returns
        -------
        val : float
              the evaluated benchmark function
        """
        self._update_check_budget()
        self._check_bounds(x)
        self.nfev += 1
        return self._func(x)

    def grad(self, x):
        """
        Gredient of the benchmark function at point x.

        Parameters
        ----------
        x : sequence
            The candidate vector for evaluating the benchmark problem. Must
            have ``len(x) == self.N``.

        Returns
        -------
        grad : sequence
              the gradient of bechmark function, have size == self.N
        """
        self._update_check_budget()
        self._check_bounds(x)
        self.ngrad += 1
        return self._grad_func(x)

    def hess(self, x):
        """
        Hessian of the benchmark function at point x.

        Parameters
        ----------
        x : sequence
            The candidate vector for evaluating the benchmark problem. Must
            have ``len(x) == self.N``.

        Returns
        -------
        H : array
              the hessian of benchmark function, have size == (self.N, size.N)
        """
        self._update_check_budget()
        self._check_bounds(x)
        self.nhess += 1
        return self._hess_func(x)

    def change_dimensions(self, ndim):
        """
        Changes the dimensionality of the benchmark problem

        The dimensionality will only be changed if the problem is suitable

        Parameters
        ----------
        ndim : int
               The new dimensionality for the problem.
        """

        if self.change_dimensionality:
            self._dimensions = ndim
        else:
            raise ValueError("dimensionality cannot be changed for this" "problem")

    @property
    def bounds(self):
        """
        The lower/upper bounds to be used for minimizing the problem.
        This a list of (lower, upper) tuples that contain the lower and upper
        bounds for the problem.  The problem should not be asked for evaluation
        outside these bounds. ``len(bounds) == N``.
        """
        if self.change_dimensionality:
            return [self._bounds[0]] * self.N
        else:
            return self._bounds

    @property
    def N(self):
        """
        The dimensionality of the problem.

        Returns
        -------
        N : int
            The dimensionality of the problem
        """
        return self._dimensions

    @property
    def xmin(self):
        """
        The lower bounds for the problem

        Returns
        -------
        xmin : sequence
            The lower bounds for the problem
        """
        return asarray([b[0] for b in self.bounds])

    @property
    def xmax(self):
        """
        The upper bounds for the problem

        Returns
        -------
        xmax : sequence
            The upper bounds for the problem
        """
        return asarray([b[1] for b in self.bounds])

    @property
    def budget_left(self):
        """
        The number of allowed function/gradient/hessian evaluations left

        Returns
        -------
        budget : int
            The number of allowed function/gradient/hessian evaluations left
        """
        return self._budget


class Adjiman(BenchmarkFunction):
    r"""
    Adjiman objective function.

    The Adjiman [1]_ global optimization problem is a multimodal minimization
    problem defined as follows:

    .. math::

        f_{\text{Adjiman}}(x) = \cos(x_1)\sin(x_2) - \frac{x_1}{(x_2^2 + 1)}


    with, :math:`x_1 \in [-1, 2]` and :math:`x_2 \in [-1, 1]`.

    *Global optimum*: :math:`f(x) = -2.02181` for :math:`x = [2.0, 0.10578]`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = ([-1.0, 2.0], [-1.0, 1.0])
        self.global_optimum = [[2.0, 0.10578]]
        self.fglob = -2.02180678

    def _func(self, x, *args):
        return cos(x[0]) * sin(x[1]) - x[0] / (x[1] ** 2 + 1)


class Alpine02(BenchmarkFunction):
    r"""
    Alpine02 objective function.

    The Alpine02 [1]_ global optimization problem is a multimodal minimization
    problem defined as follows:

    .. math::

        f_{\text{Alpine02}(x) = \prod_{i=1}^{n} \sqrt{x_i} \sin(x_i)


    Here, :math:`n` represents the number of dimensions and :math:`x_i \in [0,
    10]` for :math:`i = 1, ..., n`.

    *Global optimum*: :math:`f(x) = -6.1295` for :math:`x =
    [7.91705268, 4.81584232]` for :math:`i = 1, 2`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.

    TODO: eqn 7 in [1]_ has the wrong global minimum value.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([0.001] * self.N, [9.999] * self.N))
        self.global_optimum = [[7.91705268, 4.81584232]]
        self.fglob = -6.12950
        self.change_dimensionality = True

    def _func(self, x, *args):

        return prod(sqrt(x) * sin(x))


class Brent(BenchmarkFunction):
    r"""
    Brent objective function.

    The Brent [1]_ global optimization problem is a multimodal minimization
    problem defined as follows:

    .. math::

        f_{\text{Brent}}(x) = (x_1 + 10)^2 + (x_2 + 10)^2 + e^{(-x_1^2 -x_2^2)}


    with :math:`x_i \in [-10, 10]` for :math:`i = 1, 2`.

    *Global optimum*: :math:`f(x) = 0` for :math:`x = [-10, -10]`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.

    TODO solution is different to Jamil#24
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([-10.0] * self.N, [10.0] * self.N))
        self.custom_bounds = ([-10, 2], [-10, 2])

        self.global_optimum = [[-10.0, -10.0]]
        self.fglob = 0.0

    def _func(self, x, *args):
        return (
            (x[0] + 10.0) ** 2.0
            + (x[1] + 10.0) ** 2.0
            + exp(-x[0] ** 2.0 - x[1] ** 2.0)
        )


class Bird(BenchmarkFunction):
    r"""
    Bird objective function.

    The Bird global optimization problem is a multimodal minimization
    problem defined as follows

    .. math::

        f_{\text{Bird}}(x) = \left(x_1 - x_2\right)^{2} + e^{\left[1 -
         \sin\left(x_1\right) \right]^{2}} \cos\left(x_2\right) + e^{\left[1 -
          \cos\left(x_2\right)\right]^{2}} \sin\left(x_1\right)


    with :math:`x_i \in [-2\pi, 2\pi]`

    *Global optimum*: :math:`f(x) = -106.7645367198034` for :math:`x
    = [4.701055751981055, 3.152946019601391]` or :math:`x =
    [-1.582142172055011, -3.130246799635430]`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([-2.0 * pi] * self.N, [2.0 * pi] * self.N))
        self.global_optimum = [
            [4.701055751981055, 3.152946019601391],
            [-1.582142172055011, -3.130246799635430],
        ]
        self.fglob = -106.7645367198034

    def _func(self, x, *args):

        return (
            sin(x[0]) * exp((1 - cos(x[1])) ** 2)
            + cos(x[1]) * exp((1 - sin(x[0])) ** 2)
            + (x[0] - x[1]) ** 2
        )


class GoldsteinPrice(BenchmarkFunction):
    r"""
    Goldstein-Price objective function.

    This class defines the Goldstein-Price [1]_ global optimization problem. This
    is a multimodal minimization problem defined as follows:

    .. math::

        f_{\text{GoldsteinPrice}}(x) = \left[ 1 + (x_1 + x_2 + 1)^2
        (19 - 14 x_1 + 3 x_1^2 - 14 x_2 + 6 x_1 x_2 + 3 x_2^2) \right]
        \left[ 30 + ( 2x_1 - 3 x_2)^2 (18 - 32 x_1 + 12 x_1^2
        + 48 x_2 - 36 x_1 x_2 + 27 x_2^2) \right]


    with :math:`x_i \in [-2, 2]` for :math:`i = 1, 2`.

    *Global optimum*: :math:`f(x) = 3` for :math:`x = [0, -1]`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([-2.0] * self.N, [2.0] * self.N))

        self.global_optimum = [[0.0, -1.0]]
        self.fglob = 3.0

    def _func(self, x, *args):

        a = 1 + (x[0] + x[1] + 1) ** 2 * (
            19 - 14 * x[0] + 3 * x[0] ** 2 - 14 * x[1] + 6 * x[0] * x[1] + 3 * x[1] ** 2
        )
        b = 30 + (2 * x[0] - 3 * x[1]) ** 2 * (
            18
            - 32 * x[0]
            + 12 * x[0] ** 2
            + 48 * x[1]
            - 36 * x[0] * x[1]
            + 27 * x[1] ** 2
        )
        return a * b


class Hosaki(BenchmarkFunction):
    r"""
    Hosaki objective function.

    This class defines the Hosaki [1]_ global optimization problem. This
    is a multimodal minimization problem defined as follows:

    .. math::

        f_{\text{Hosaki}}(x) = \left ( 1 - 8 x_1 + 7 x_1^2 - \frac{7}{3} x_1^3
        + \frac{1}{4} x_1^4 \right ) x_2^2 e^{-x_1}


    with :math:`x_i \in [0, 10]` for :math:`i = 1, 2`.

    *Global optimum*: :math:`f(x) = -2.3458115` for :math:`x = [4, 2]`.

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = ([0.0, 5.0], [0.0, 6.0])
        self.custom_bounds = [(0, 5), (0, 5)]

        self.global_optimum = [[4, 2]]
        self.fglob = -2.3458115

    def _func(self, x, *args):

        val = 1 - 8 * x[0] + 7 * x[0] ** 2 - 7 / 3.0 * x[0] ** 3 + 0.25 * x[0] ** 4
        return val * x[1] ** 2 * exp(-x[1])


class Keane(BenchmarkFunction):
    r"""
    Keane objective function.

    This class defines the Keane [1]_ global optimization problem. This
    is a multimodal minimization problem defined as follows:

    .. math::

        f_{\text{Keane}}(x) = \frac{\sin^2(x_1 - x_2)\sin^2(x_1 + x_2)}
        {\sqrt{x_1^2 + x_2^2}}


    with :math:`x_i \in [0, 10]` for :math:`i = 1, 2`.

    *Global optimum*: :math:`f(x) = 0.0` for
    :math:`x = [7.85396153, 7.85396135]`.

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.

    TODO: Jamil #69, there is no way that the function can have a negative
    value.  Everything is squared.  I think that they have the wrong solution.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([0.001] * self.N, [9.999] * self.N))

        self.global_optimum = [[7.85396153, 7.85396135]]
        self.custom_bounds = [(-1, 0.34), (-1, 0.34)]
        self.fglob = 0.0

    def _func(self, x, *args):

        val = sin(x[0] - x[1]) ** 2 * sin(x[0] + x[1]) ** 2
        return val / sqrt(x[0] ** 2 + x[1] ** 2)


class Levy03(BenchmarkFunction):
    r"""
    Levy 3 objective function.

    This class defines the Levy 3 [1]_ global optimization problem. This
    is a multimodal minimization problem defined as follows:

    .. math::

        f_{\text{Levy03}}(\mathbf{x}) =
        \sin^2(\pi y_1)+\sum_{i=1}^{n-1}(y_i-1)^2[1+10\sin^2(\pi y_{i+1})]+(y_n-1)^2

    Where, in this exercise:

    .. math::

        y_i=1+\frac{x_i-1}{4}


    Here, :math:`n` represents the number of dimensions and
    :math:`x_i \in [-10, 10]` for :math:`i=1,...,n`.

    *Global optimum*: :math:`f(x_i) = 0` for :math:`x_i = 1` for :math:`i=1,...,n`

    .. [1] Mishra, S. Global Optimization by Differential Evolution and
    Particle Swarm Methods: Evaluation on Some Benchmark Functions.
    Munich Personal RePEc Archive, 2006, 1005

    TODO: not clear what the Levy function definition is.  Gavana, Mishra,
    Adorio have different forms. Indeed Levy 3 docstring from Gavana
    disagrees with the Gavana code!  The following code is from the Mishra
    listing of Levy08.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([-10.0] * self.N, [10.0] * self.N))
        self.custom_bounds = [(-5, 5), (-5, 5)]

        self.global_optimum = [[1 for _ in range(self.N)]]
        self.fglob = 0.0

    def _func(self, x, *args):
        self.nfev += 1

        y = 1 + (x - 1) / 4
        v = sum((y[:-1] - 1) ** 2 * (1 + 10 * sin(pi * y[1:]) ** 2))
        z = (y[-1] - 1) ** 2
        return sin(pi * y[0]) ** 2 + v + z


class Levy03_2D(Levy03):
    """
    Levy 3 objective function in 2D. See
    :class:`Levy03` for more details.
    """

    def __init__(self, budget):
        Levy03.__init__(self, budget, dimensions=2)


class Levy03_3D(Levy03):
    """
    Levy 3 objective function in 3D. See
    :class:`Levy03` for more details.
    """

    def __init__(self, budget):
        Levy03.__init__(self, budget, dimensions=3)


class Levy03_4D(Levy03):
    """
    Levy 3 objective function in 4D. See
    :class:`Levy03` for more details.
    """

    def __init__(self, budget):
        Levy03.__init__(self, budget, dimensions=4)


class Qing(BenchmarkFunction):
    r"""
    Qing objective function.

    This class defines the Qing [1]_ global optimization problem. This is a
    multimodal minimization problem defined as follows:

    .. math::

        f_{\text{Qing}}(x) = \sum_{i=1}^{n} (x_i^2 - i)^2


    Here, :math:`n` represents the number of dimensions and
    :math:`x_i \in [-500, 500]` for :math:`i = 1, ..., n`.

    *Global optimum*: :math:`f(x) = 0` for :math:`x_i = \pm \sqrt(i)` for
    :math:`i = 1, ..., n`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([-500.0] * self.N, [500.0] * self.N))
        self.custom_bounds = [(-2, 2), (-2, 2)]
        self.global_optimum = [[sqrt(_) for _ in range(1, self.N + 1)]]
        self.fglob = 0
        self.change_dimensionality = True

    def _func(self, x, *args):

        i = arange(1, self.N + 1)
        return sum((x**2.0 - i) ** 2.0)


class Qing_2D(Qing):
    """
    Qing objective function in 2D. See
    :class:`Qing` for more details.
    """

    def __init__(self, budget):
        Qing.__init__(self, budget, dimensions=2)


class Qing_3D(Qing):
    """
    Qing objective function in 3D. See
    :class:`Qing` for more details.
    """

    def __init__(self, budget):
        Qing.__init__(self, budget, dimensions=3)


class Qing_4D(Qing):
    """
    Qing objective function in 4D. See
    :class:`Qing` for more details.
    """

    def __init__(self, budget):
        Qing.__init__(self, budget, dimensions=4)


class Ripple25(BenchmarkFunction):
    r"""
    Ripple 25 objective function.

    This class defines the Ripple 25 [1]_ global optimization problem. This is a
    multimodal minimization problem defined as follows:

    .. math::

        f_{\text{Ripple25}}(x) = \sum_{i=1}^2 -e^{-2
        \log 2 (\frac{x_i-0.1}{0.8})^2}
        \left[\sin^6(5 \pi x_i) \right]


    Here, :math:`n` represents the number of dimensions and
    :math:`x_i \in [0, 1]` for :math:`i = 1, ..., n`.

    *Global optimum*: :math:`f(x) = -2` for :math:`x_i = 0.1` for
    :math:`i = 1, ..., n`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)
        self._bounds = list(zip([0.0] * self.N, [1.0] * self.N))

        self.global_optimum = [[0.1 for _ in range(self.N)]]
        self.fglob = -2.0

    def _func(self, x, *args):

        u = -2.0 * log(2.0) * ((x - 0.1) / 0.8) ** 2.0
        v = sin(5.0 * pi * x) ** 6.0
        return sum(-exp(u) * v)


class Schwefel22(BenchmarkFunction):
    r"""
    Schwefel 22 objective function.

    This class defines the Schwefel 22 [1]_ global optimization problem. This
    is a multimodal minimization problem defined as follows:

    .. math::

        f_{\text{Schwefel22}}(x) = \sum_{i=1}^n \lvert x_i \rvert
                                  + \prod_{i=1}^n \lvert x_i \rvert


    Here, :math:`n` represents the number of dimensions and
    :math:`x_i \in [-100, 100]` for :math:`i = 1, ..., n`.

    *Global optimum*: :math:`f(x) = 0` for :math:`x_i = 0` for
    :math:`i = 1, ..., n`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)
        self._bounds = list(zip([-100.0] * self.N, [100.0] * self.N))
        self.custom_bounds = ([-10.0, 10.0], [-10.0, 10.0])

        self.global_optimum = [[0.0 for _ in range(self.N)]]
        self.fglob = 0.0
        self.change_dimensionality = True

    def _func(self, x, *args):

        return sum(abs(x)) + prod(abs(x))


class Schwefel22_2D(Schwefel22):
    """
    Schwefel 22 objective function in 2D. See
    :class:`Schwefel22` for more details.
    """

    def __init__(self, budget):
        Schwefel22.__init__(self, budget, dimensions=2)


class Schwefel22_3D(Schwefel22):
    """
    Schwefel 22 objective function in 3D. See
    :class:`Schwefel22` for more details.
    """

    def __init__(self, budget):
        Schwefel22.__init__(self, budget, dimensions=3)


class Schwefel22_4D(Schwefel22):
    """
    Schwefel 22 objective function in 4D. See
    :class:`Schwefel22` for more details.
    """

    def __init__(self, budget):
        Schwefel22.__init__(self, budget, dimensions=4)


class StyblinskiTang(BenchmarkFunction):
    r"""
    StyblinskiTang objective function.

    This class defines the Styblinski-Tang [1]_ global optimization problem. This
    is a multimodal minimization problem defined as follows:

    .. math::

       f_{\text{StyblinskiTang}}(x) = \sum_{i=1}^{n} \left(x_i^4
                                       - 16x_i^2 + 5x_i \right)

    Here, :math:`n` represents the number of dimensions and
    :math:`x_i \in [-5, 5]` for :math:`i = 1, ..., n`.

    *Global optimum*: :math:`f(x) = -39.16616570377142n` for
    :math:`x_i = -2.903534018185960` for :math:`i = 1, ..., n`

    .. [1] Jamil, M. & Yang, X.-S. A Literature Survey of Benchmark Functions
    For Global Optimization Problems Int. Journal of Mathematical Modelling
    and Numerical Optimisation, 2013, 4, 150-194.
    """

    def __init__(self, budget, dimensions=2):
        BenchmarkFunction.__init__(self, budget, dimensions)

        self._bounds = list(zip([-5.0] * self.N, [5.0] * self.N))

        self.global_optimum = [[-2.903534018185960 for _ in range(self.N)]]
        self.fglob = -39.16616570377142 * self.N
        self.change_dimensionality = True

    def _func(self, x, *args):

        return sum(x**4 - 16 * x**2 + 5 * x) / 2


class StyblinskiTang_2D(StyblinskiTang):
    """
    StyblinskiTang objective function in 2D. See
    :class:`StyblinskiTang` for more details.
    """

    def __init__(self, budget):
        StyblinskiTang.__init__(self, budget, dimensions=2)


class StyblinskiTang_3D(StyblinskiTang):
    """
    StyblinskiTang objective function in 3D. See
    :class:`StyblinskiTang` for more details.
    """

    def __init__(self, budget):
        StyblinskiTang.__init__(self, budget, dimensions=3)


class StyblinskiTang_4D(StyblinskiTang):
    """
    StyblinskiTang objective function in 4D. See
    :class:`StyblinskiTang` for more details.
    """

    def __init__(self, budget):
        StyblinskiTang.__init__(self, budget, dimensions=4)


BENCHMARK_FUNCTIONS = [
    Adjiman,
    Alpine02,
    Brent,
    Bird,
    GoldsteinPrice,
    Hosaki,
    Keane,
    Levy03_2D,
    Levy03_3D,
    Levy03_4D,
    Qing_2D,
    Qing_3D,
    Qing_4D,
    Ripple25,
    Schwefel22_2D,
    Schwefel22_3D,
    Schwefel22_4D,
    StyblinskiTang_2D,
    StyblinskiTang_3D,
    StyblinskiTang_4D,
]
