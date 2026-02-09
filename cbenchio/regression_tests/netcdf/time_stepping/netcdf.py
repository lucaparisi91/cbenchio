"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio
import h5py

@rfm.simple_test
class NETCDFTimeStepping(Cbenchio):
    """Tests the netcdf API of benchio"""
    
    executable_opts = ["netcdf.yaml" ]
    tasks = 4

    @sanity_function
    def validate_chunking(self):
        """ Check that the data was chunked as expected """
        # Data file created in stage directory
        file_name=f"{self.stagedir}/data/data_pool_0.outT0"
        f = h5py.File(file_name, 'r')
        shape = f["data0"].shape
        expected_shape = (10,1,1000,40)
        return sn.assert_eq(shape, expected_shape)
