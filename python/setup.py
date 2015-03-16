from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize
import numpy as np

INC = ['..', '../tree']
LIB = ['..', '../tree']

ext_modules = [
    Extension("tree",
              sources=["kestrels/tree.pyx","../tree/tree_kernel.cpp","../tree/tree.cpp"],
              include_dirs=INC + [np.get_include()],
              library_dirs=LIB,
              extra_compile_args=['-fopenmp','-std=c++0x','-ftree-vectorize'],
              extra_link_args=['-fopenmp','-std=c++0x','-lboost_regex'],
              language="c++",
          ),          
]

setup(
    name='kestrels',
    ext_modules=cythonize(ext_modules),
    packages=['kestrels'],
    cmdclass = {'build_ext': build_ext},
)
