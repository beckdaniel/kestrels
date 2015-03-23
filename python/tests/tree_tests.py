
import unittest
import numpy as np
from kestrels import SASSTreeKernel

class SASSTreeKernelTests(unittest.TestCase):
    
    def test_K(self):
        X = np.array([['(S (NP ns) (VP v))'],
                      ['(S (NP n) (VP v))'],
                      ['(S (NP (N a)) (VP (V c)))'],
                      #['(S (NP (Det a) (N b)) (VP (V c)))'],
                      #['(S (NP (ADJ colorless) (N ideas)) (VP (V sleep) (ADV furiously)))'],
                      ],
                     dtype=object)
        k = SASSTreeKernel(_lambda=1)
        target = k.K(X)
        result = [[ 1.,          0.5,         0.10540926,  0.08333333,  0.06711561],
                  [ 0.5,         1.,          0.10540926,  0.08333333,  0.06711561],
                  [ 0.10540926,  0.10540926,  1.,          0.31622777,  0.04244764],
                  [ 0.08333333,  0.08333333,  0.31622777,  1.,          0.0335578 ],
                  [ 0.06711561,  0.06711561,  0.04244764,  0.0335578,   1.        ]]
        print target
        self.assertAlmostEqual(np.sum(result), np.sum(target))


if __name__ == "__main__":
    unittest.main()
