"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio


@rfm.simple_test
class hdf5(Cbenchio):
    """Tests the hdf5 API of benchio"""
    
    executable_opts = ["hdf5.yaml" ]
    num_tasks = 4