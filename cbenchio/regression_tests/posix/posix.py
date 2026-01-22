"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio

@rfm.simple_test
class Posix(Cbenchio):
    """Tests the posix API of benchio"""
    
    executable_opts = ["posix.yaml" ]
    tasks = 4

@rfm.simple_test
class PosixDirect(Cbenchio):
    """Tests the posix direct API of benchio"""
    
    executable_opts = ["posixDirect.yaml" ]
    tasks = 4

@rfm.simple_test
class PosixStrided(Cbenchio):
    """Tests the posix API of benchio using strided access pattern"""
    
    executable_opts = ["posixStrided.yaml" ]
    tasks = 4

@rfm.simple_test
class PosixLockAhead(Cbenchio):
    """Tests the posix API of benchio using strided access pattern"""
    
    executable_opts = ["posixLockAhead.yaml" ]
    tasks = 4