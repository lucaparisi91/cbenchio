# Lammps benchmark

You can change the number of particles in the simulation with 

```bash
module load cray-python
python generate_initial_conditions.py -n ${NPARTICLES} > initial_configuration.txt
```

The lammps configuration is contained in the many_particles.lam file.
You can change the frequency and the format of the output by editing the lines similar to 

```bash
dump dum1 all custom 200 dump_output.dat id type x y z ix iy iz 
dump h5md1 all h5md 200 dump_output.hdf5 position image
dump netcdfmd1 all netcdf/mpiio 200 dump_output.nc id type x y z
```

The above will dump to three files every 200 time steps, a serial text file, a serial hdf5 file and a parallel netcdf file.
