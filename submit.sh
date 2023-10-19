#!/bin/bash

# Slurm job options (name, compute nodes, job time)
#SBATCH --job-name=test_write
#SBATCH --time=00-00:10:00
#SBATCH --exclusive
#SBATCH --nodes=4
#SBATCH --partition=standard
#SBATCH --exclusive
#SBATCH --account=z04
#SBATCH  -q standard
#SBATCH --tasks-per-node=2
#SBATCH --cpus-per-task=1


module load gcc/10.2.0
module load openmpi

rm *.out

srun  ./test