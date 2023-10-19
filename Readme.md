# I/O Benchmarking tutorial

Goal is to profile several codes of increasing complexity.
Some material on using Darshan on Archer2 should be produced.

# Outline

3D array grid and profile with darshan trough different optimizations

1. POSIX write per process
2. MPI-IO independent ( suffers from file locking ? )
3. MPI-IO collective ( file locking issue eased )
4. Changing the number of stripes ( OST bandwith limit)
4. MPI-IO HDF5 (  similar scaling as MPI-IO but with an overhead from many small metadata writes )
5. MPI-IO Adios2 ( scaling should be much improved )

Possible other factors to explore
- tuning environment variables
- looking at verbose output of MPI-IO
- write 1D contigous array ( no file locking issues )

# Materials
- Documentation on how to use darshan
- Slides for following throughout the stack

# More information

- https://prace-ri.eu/wp-content/uploads/Best-Practice-Guide_Parallel-IO.pdf
- https://hps.vi4io.org/events/2022/start