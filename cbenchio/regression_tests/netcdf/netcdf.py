"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio


@rfm.simple_test
class netcdf(Cbenchio):
    """Tests the netcdf API of benchio"""
    
    executable_opts = ["netcdf.yaml" ]
    tasks = 4


