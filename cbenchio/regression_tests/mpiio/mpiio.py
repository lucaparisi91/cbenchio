"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio

@rfm.simple_test
class Mpiio(Cbenchio):
    """Tests the mpiio API of benchio"""
    
    executable_opts = ["mpiio.yaml" ]
    tasks = 4
    
@rfm.simple_test
class MpiioPools(Cbenchio):
    """Tests the mpiio API of benchio. Uses multiple pools."""
    
    executable_opts = ["mpiioPools.yaml" ]
    tasks = 4
