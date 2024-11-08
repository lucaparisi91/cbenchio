# MPIO performance

We will be using cbenchio to investigate the performance of parallel I/O. 

Submit the job using using the `submit.sh` script.
The `config.yaml` file can be used to tune the mpio configuration.

Create the directory where to write the data. This is specified by the path variable in the `config.yaml` file.

```bash
sbatch submit.sh
```

You should see a `slurm-<jobid>.out` appear and a `report.yaml` with some basic performance characteristics.

## Check bandwidth and performance 

Try to change the yaml configuration file and see how the performance improve. Look at the darshan profiles and traces. How have these changed.

- Try to change the overall size of the array . How does the performance improve with size ?
- Try to change the processor grid distribution, but keeping the tatal number of processors unchanged. The size of an array can be computed as (total number of elements in the array) * 8 bytes. What is an optimal processor distribution, for a 16GB array ?
- Try to change the striping of the directory to fully striped and repeat the steps above . For instance try to fully stripe the directory with  `lfs setstripe -c -1 data`. Do you see any difference in the bandwidth ?

## Darshan profiling

Repeat the steps above , but obtain a performance profile. Check both a dxt trace and a summary profile. Can you exaplain the results you saw ? 
See below a few suggestions for what to look for

- What is the MPIIO access size ?
- What is the POSIX access size ?
- How many POSIX write/read operations do you see ?
- Which ranks are doing POSIX calls ? Which ranks are doing MPI calls ?
