import numpy as np
cimport numpy as np
from GPy.kern import Kern
from GPy.core.parameterization import Param
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp cimport bool

DTYPE = np.double
ctypedef np.double_t DTYPE_t

cdef extern from "tree/tree_kernel.hpp":
    cdef cppclass KernelResult:
        double k
        vector[double] dlambda
        vector[double] dalpha

    ctypedef vector[KernelResult] VecResult

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
        void Kdiag(const vector[string]& trees,
                   vector[KernelResult]& result)
        void K(const vector[string]& trees,
               vector[VecResult]& result)
        void K(const vector[string]& trees1,
               const vector[string]& trees2,
               vector[VecResult]& result)

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
        if X2 == None:
            X2 = X
        return self.kernel.K(X, X2)

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
        #return 10
        #if (X2 == None and (self._lambda == self.kernel._lambda).all() 
        #    and (self._sigma == self.kernel._sigma).all()):
        #    return self.result
        #self.kernel._lambda = self._lambda.copy()
        #self.kernel._sigma = self._sigma.copy()
        
        cdef vector[string] X_cpp = _convert_input(X)
        cdef vector[string] X2_cpp = _convert_input(X2)
        cdef vector[VecResult] result_cpp;
        #for i in range(X_cpp.size()):
        #    print X_cpp[i]
        self.kernel.K(X_cpp, X_cpp, result_cpp)
        #result = 10
        #dl = np.array([[[0.5] * len(X)] * len(X2)])
        #ds = np.array([[[0.5] * len(X)] * len(X2)])
        #cdef np.ndarray[DTYPE_t, ndim=3] result = np.zeros(shape=(len(X), len(X2), 1))
        #cdef double[:,:,:] result_view = result
        #_convert_result(result_cpp, result_view)
        #self.dlambda = dl
        #self.dsigma = ds
        #return self.result
        return 10

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

###################

cdef vector[string] _convert_input(X):
    cdef vector[string] X_cpp;
    for row in X:
        X_cpp.push_back(row[0])
    return X_cpp

cdef void _convert_result(vector[VecResult] result_cpp, double[:,:,:] result):
    cdef VecResult vec_result
    cdef KernelResult kernel_result
    for i in range(result_cpp.size()):
        vec_result = result_cpp[i]
        for j in range(vec_result.size()):
            kernel_result = vec_result[j]
            result[i][j][0] = kernel_result.k
        
