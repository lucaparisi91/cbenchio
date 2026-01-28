"""ReFrame base module for LAMMPS tests"""

import os
import reframe as rfm
import reframe.utility.sanity as sn
from regression_tests.base.cbenchio import Cbenchio
import h5py

@rfm.simple_test
class hdf5_chunks(Cbenchio):
    """Tests the hdf5 API of benchio"""
    executable_opts = ["hdf5_chunking.yaml" ]
    num_tasks = 4

    @sanity_function
    def validate_chunking(self):
        """ Check that the data was chunked as expected """
        # Data file created in stage directory
        file_name=f"{self.stagedir}/data/data_pool_0.outT0"
        f = h5py.File(file_name, 'r')
        chunks = list( f["data0"].iter_chunks() )
        return sn.assert_eq(chunks[0], (slice(0, 1, 1), slice(0, 100, 1), slice(0, 10, 1))) and sn.assert_gt(len(chunks), 1)


