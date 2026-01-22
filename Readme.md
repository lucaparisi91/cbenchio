# Cbenchio

This is a simple program to test I/O performance when writing a 2D array to disk.
Currently supports POSIX, MPIIO and HDF5 interface. 
The only tested filesystem is lustre.

## Building 

The build system uses cmake.

```bash
mkdir build
cd build
cmake ../cbenchio
make 
make install
```

Dependencies are:

- hdf5 ( required )
- netcdf version >=4 (required )
- Reframe (optional, only needed for running the integration tests )

## Configuration

The output is configured using a yaml file.
An example configuration file can found in config.yaml .

- API: string
    Allowed values: hdf5, posix, mpio
- path: string
    - Path where to write to the file. The directory should exist
- isCollective[true]: bool
    - Only falid for parallel io. Wether to use collective or indipendent MPI calls.
- filePerProcess: bool
    - If false write to a single file. Otherwise write to multiple file
- shape: int array
    - Shape of the 3D array to write to memory. The length of the array must be 3.
- processorGid[[0,0,0]]: int array
    - Shape of the procressor grid. The length of the array must be 3.
- operation: string
    Either "read" or "write    
- direct[false]: bool
    - Perform direct I/O
- alignment[0]: int
    - Allocates memory aligned array. The read/write posix calls will be splits in calls of max size equal to the alignment
- lockAhead[false]: bool
    - Uses lustre lockahead feature
- waitLockAhead[false]: bool
    - Wait for a fixed amount of time to see if the client gets the requested locks. Prints how many locks were granted when the waiting period expires
- lockNoExpand[false] : bool
    - Advise to not expand locks on the whole file
- chunkSize[0]: int
    - Number of bytes to read/write per operation
- strided[false] : bool
    - Chunks written by different processors are interleaved.
- poolSize[nranks(MPI_COMM_WORLD)] : int
    - Split the ranks in pools , each with a communicator of size poolSize.
    - The data is read/written by all pools concurrently. Thus the total size read/written is equal to the size of the array indicated by shape argument multipled by the number of pools

## Build on Archer2

See the archer2 build script in scripts directory

# Run tests on Archer2

The applications contains unit tests.
You can run them running `ctest` from the build directory. The unit tests assume the usage of a slurm scheduler.

There are also regression tests. You can use them using Reframe.

Assuming `BENCHIO_ROOT` is the path to the root of this repo on the filesystem

```bash
TESTS_ROOT=$BENCHIO_ROOT/cbenchio/regression_tests
export PYTHONPATH=$BENCHIO_ROOT/cbenchio:$PYTHONPATH
reframe -R -C  $TESTS_ROOT/platform/cirrus-ex.py -c $TESTS_ROOT -S benchio_exe="/work/z19/shared/lparisi/cbenchio/opt/cbenchio/0.1/bin/benchio" -r
```


