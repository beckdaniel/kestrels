import numpy as np
from GPy.kern import Kern
from GPy.core.parameterization import Param
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp cimport bool

cdef extern from "tree/tree_kernel.hpp":
    cdef cppclass SymbolAwareSubsetTreeKernel:
        SymbolAwareSubsetTreeKernel() except +
        SymbolAwareSubsetTreeKernel(const vector[double]& _lambda, const bool normalize) except +
        SymbolAwareSubsetTreeKernel(const vector[double]& _lambda,
                                    const vector[double]& alpha, const bool normalize)
        SymbolAwareSubsetTreeKernel(const vector[double]& _lambda,
                                    const vector[double]& alpha, const bool normalize,
                                    const map[string, int]& lambda_buckets)
        SymbolAwareSubsetTreeKernel(const vector[double]& _lambda,
                                    const vector[double]& alpha, const bool normalize,
                                    const map[string, int]& lambda_buckets,
                                    const map[string, int]& alpha_buckets)

class SASSTreeKernel(Kern):
    def __init__(self, _lambda=np.array([0.5]), _alpha=np.array([1.0]), lambda_buckets={}, alpha_buckets={},
                 normalize=True, active_dims=None, num_threads=1, parallel=True):
        super(SASSTreeKernel, self).__init__(1, active_dims, 'sasstk')
        self.normalize = normalize
        self._lambda = Param('lambda', _lambda)
        self._alpha = Param('alpha', _alpha)
        self.link_parameters(self._lambda, self._alpha)
        self.kernel = SASSTreeKernelWrapper(_lambda, _alpha, lambda_buckets, alpha_buckets, normalize)
        # just to ensure parameters are different at the first time
        #self.kernel._lambda = _lambda.copy() + 1
        #self.kernel._alpha = _alpha.copy() + 1

    def K(self, X, X2):
        return self.kernel.k(X, X2)

    def Kdiag(self, X):
        return self.kernel.Kdiag(X)

    def update_gradients_full(self, dL_dK, X, X2):
        self.kernel.update_gradients_full(self, dL_dK, X, X2)

cdef class SASSTreeKernelWrapper:
    """
    An extension of SST, including specific lambdas/alphas for each symbol.
    """
    cdef SymbolAwareSubsetTreeKernel* kernel

    def __cinit__(self, _lambda=np.array([0.5]), _alpha=np.array([1.0]), lambda_buckets={}, alpha_buckets={},
                 normalize=True):
        #self.normalize = normalize
        cdef vector[double] lambda2
        cdef bool norm = True
        lambda2.push_back(0.5)
        #self.kernel = new SymbolAwareSubsetTreeKernel()
        self.kernel = new SymbolAwareSubsetTreeKernel(lambda2, norm)
        #self.kernel = cy_sa_tree.SymbolAwareSubsetTreeKernel(_lambda, _sigma, lambda_buckets, sigma_buckets,
        #                                                  normalize, num_threads=num_threads, parallel=parallel)
        # just to ensure parameters are different at the first time
        #self.kernel._lambda = _lambda.copy() + 1
        #self.kernel._sigma = _sigma.copy() + 1

    def K(self, X, X2):
        return 10
        #if (X2 == None and (self._lambda == self.kernel._lambda).all() 
        #    and (self._sigma == self.kernel._sigma).all()):
        #    return self.result
        #self.kernel._lambda = self._lambda.copy()
        #self.kernel._sigma = self._sigma.copy()
        #result, dl, ds = self.kernel.K(X, X2)
        #result = 10
        #dl = np.array([[[0.5] * len(X)] * len(X2)])
        #ds = np.array([[[0.5] * len(X)] * len(X2)])
        #self.result = result
        #self.dlambda = dl
        #self.dsigma = ds
        #return self.result

    def Kdiag(self, X):
        return 20
        #self.kernel._lambda = self._lambda.copy()
        #self.kernel._sigma = self._sigma.copy()
        #if self.normalize:
        #    return np.ones(X.shape[0])
        #else:
        #    return self.kernel.Kdiag(X)

    def update_gradients_full(self, dL_dK, X, X2):
        #self._lambda.gradient = np.array([np.sum(self.dlambda[:,:,i] * dL_dK)
        #                                  for i in range(len(self._lambda))])
        #self._sigma.gradient = np.array([np.sum(self.dsigma[:,:,i] * dL_dK)
        #                                 for i in range(len(self._sigma))])
        pass
