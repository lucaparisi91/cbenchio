# Scorep profiling

Try to visualize a profile made with scorep.

- Submit the provide sbatch script `sbatch submit.sh` . This should create a scorep folder in your current directory.

- Load the right environment

```bash
module use /work/z19/shared/lparisi/courses/io/modules
module load cbenchio-scorep
```

Visualise the profile with the square command

```bash
square scorep-benchio
```

