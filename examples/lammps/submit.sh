#!/bin/bash

#SBATCH --job-name=lammps_io
#SBATCH --time=0:20:0
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=1
#SBATCH --partition=standard
#SBATCH --qos=short

export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK

export OMP_NUM_THREADS=1

module use /work/z19/shared/lparisi/courses/io/modules
module load lammps
module load darshan/3.4.6

#export  DARSHAN_DISABLE_SHARED_REDUCTION=1 
#export DXT_ENABLE_IO_TRACE=1
#export MPICH_MPIIO_STATS=1

srun --hint=nomultithread --distribution=block:block lmp -i many_particles.lam