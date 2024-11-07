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



```bash
module use /work/z19/shared/lparisi/courses/io/modules
module load darshan/3.4.6
```

- Try to change the overall size of the array . How does the performance improve with size ?
 - Try to change the processor grid distribution, but keeping the tatal number of processors unchanged. What is an optimal processor distribution, for a 16BG array ?
