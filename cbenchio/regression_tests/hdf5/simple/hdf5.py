"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio
import h5py
import numpy as np

@rfm.simple_test
class hdf5(Cbenchio):
    """Tests the hdf5 API of benchio"""
    
    executable_opts = ["hdf5.yaml" ]
    num_tasks = 4

@rfm.simple_test
class hdf5Random(Cbenchio):
    """Tests the hdf5 API of benchio using random data pattern"""
    
    executable_opts = ["hdf5Random.yaml" ]
    num_tasks = 4

    @sanity_function
    def validate_chunking(self):
        """ Check that the data was chunked as expected """
        # Data file created in stage directory
        file_name=f"{self.stagedir}/data/data_pool_0.outT0"
        f = h5py.File(file_name, 'r')
        
        return sn.assert_bounded(np.max(f["data0"]),lower=0, upper=1,msg="Random data should between 0 and 1") and sn.assert_bounded(np.min(f["data0"]),lower=0, upper=1,msg="Random data should between 0 and 1") and self.assert_successful_completion()