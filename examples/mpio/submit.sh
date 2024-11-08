#!/bin/bash

# Slurm job options (job-name, compute nodes, job time)
#SBATCH --job-name=cbenchio
#SBATCH --time=0:20:0
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=10
#SBATCH --cpus-per-task=1
#SBATCH --partition=standard
#SBATCH --qos=short

# Set the number of threads to 1
#   This prevents any threaded system libraries from automatically
#   using threading.
export OMP_NUM_THREADS=1

# Propagate the cpus-per-task setting from script to srun commands
#    By default, Slurm does not propagate this setting from the sbatch
#    options to srun commands in the job script. If this is not done,
#    process/thread pinning may be incorrect leading to poor performance
export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK

module use /work/z19/shared/lparisi/courses/io/modules
module load darshan/3.4.6
module load cbenchio

#export  DARSHAN_DISABLE_SHARED_REDUCTION=1 
#export DXT_ENABLE_IO_TRACE=1
#export MPICH_MPIIO_STATS=1

srun --distribution=block:block --hint=nomultithread benchio
