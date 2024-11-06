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




