"""ReFrame base module for LAMMPS tests"""

import os

import reframe as rfm
import reframe.utility.sanity as sn



class Cbenchio(rfm.RunOnlyRegressionTest):
    """Base class for all cbenchio regression tests"""

    valid_systems = ['cirrus-ex:compute']
    valid_prog_environs = ['PrgEnv-gnu']
    benchio_exe = variable(str, value="benchio")
    data_folder = variable(str, value="data")

    @sanity_function
    def assert_successful_completion(self):
        return sn.assert_found(r'Done',
                               self.stdout)

    @run_before('run')
    def create_data_dir(self):
        self.data_dir = os.path.join(self.current_system.resourcesdir,
                                     'cbenchio_data')
        self.prerun_cmds = [f'mkdir -p {self.data_folder}']

    @run_after('setup')
    def set_executable(self):
        self.executable = self.benchio_exe
    

