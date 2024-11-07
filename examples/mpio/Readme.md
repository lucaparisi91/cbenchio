# MPIO performance

We will be using cbenchio to investigate the performance of parallel I/O. 

Submit the job using using the `submit.sh` script.
The `config.yaml` file can be used to tune the mpio configuration.

Create the directory where to write the data. This is specified by the path variable in the `config.yaml` file.

```bash
sbatch submit.sh
```

You should see a `slurm-<jobid>.out` appear and a `report.yaml` with some basic performance characteristics.

Load darshan and check in which folder the logs have been saved. 
You should see an ouput similar to the one below. The example below that the the log was saved in the directory `$DARSHAN_LOG_DIR_PATH` ( if defined ) , otherwise the log will be saved in `/work/z19/shared/lparisi/courses/io/opt/darshan/3.4.6/darshan-logs` .

```bash
ln01:mpio$ darshan-config --log-path
$DARSHAN_LOG_DIR_PATH,/work/z19/shared/lparisi/courses/io/opt/darshan/3.4.6/darshan-logs
```

Identify the darshan file and copy it over in your current directory.
Then generate a summary file and inspect the file.

- What is the MPIIO access size ?
- What is the POSIX access size ?
- How many POSIX write/read operations do you see ?


```bash
module use /work/z19/shared/lparisi/courses/io/modules
module load darshan/3.4.6
```

Try to change the yaml configuration file and see how the performance improve. Look at the darshan profiles and traces. How have these changed.

- Try to change the overall size of the array . How does the performance improve with size ?
 - Try to change the processor grid distribution, but keeping the tatal number of processors unchanged. What is an optimal processor distribution, for a 16BG array ?

Try to changed the striping of the directory to fully striped and repeat the steps above . Do you see any difference in the profiles ?

```bash
lfs setstripe -c -1 dataDir
```
