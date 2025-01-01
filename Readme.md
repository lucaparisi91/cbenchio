# Cbenchio

This is a simple program to test I/O performance when writing a 2D array to disk.
Currently supports POSIX, MPIIO and HDF5 interface. 
The only tested filesystem is lustre.

## Configuration

The output is configured using a yaml file.
An example configuration file can found in config.yaml .

- API: string
    Allowed values: hdf5, posix, mpio
- path: string
    - Path where to write to the file. The directory should exist
- isCollective: bool
    - Only falid for parallel io. Wether to use collective or indipendent MPI calls.
- filePerProcess: bool
    - If false write to a single file. Otherwise write to multiple file
- shape: array int of length 3
    - Shape of the 3D array to write to memory
- processorGid: array of 3 integers, default [0,0,0]
    - Processor grid size
- operation: string
    Either "read" or "write    
- direct: bool, default is false
    Perform direct I/O
- alignment: bool, default is zero
    Allocates memory aligned array. The read/write posix calls will be splits in calls of max size equal to the alignment
- lockAhead[ false]: bool
    - Uses lustre lockahead feature
- waitLockAhead[false]: bool
    - Wait for a fixed amount of time to see if the client gets the requested locks. Prints how many locks were granted when the waiting period expires
- lockNoExpand[false] : bool
    - Advise to not expand locks on the whole file
- chunkSize[0]: int
    - Number of bytes to read/write per operation
- strided[false] : bool
    - Chunks written by different processors are interleaved.

## Build on Archer2

See the archer2 build script in scripts directory

# Run tests on Archer2

The applications contains unit tests.
You can run them running `ctest` from the build directory. 
See the scripts directory for an example of how to run the tests on Archer2.


