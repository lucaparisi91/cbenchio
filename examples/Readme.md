# Examples

This folder contains several examples to profile with Darshan.
Look in the folder for instructions on how to run the examples and a list of questions to answer. 


# Profiling with Darshan

Load darshan and check in which folder the logs have been saved. 

```bash
module use /work/z19/shared/lparisi/courses/io/modules
module load darshan/3.4.6
```

Darshan is configured so that logs are saved in the folder defined in  `$DARSHAN_LOG_DIR_PATH`.

You can check which directories darshan look into when saving logs with the command `darshan-config --log-path`.

### Summary reports
You can generate a summary report using the `darshan-parser` command. For instance

```bash
darshan-parser $DARSHAN_LOG_DIR_PATH/lparisi_benchio_id7999586-88755_11-8-32949-14036386306537802543_1.darshan | less
```

### Traces 
Trace data is not collected by default. The feature needs to be enabled before submitting the simulation by setting an environment variable.

```bash
export DXT_ENABLE_IO_TRACE=1
```

You can generate a trace  by using the `darshan-dxt-parser` command. 

```bash
darshan-dxt-parser $DARSHAN_LOG_DIR_PATH/lparisi_benchio_id7999586-88755_11-8-32949-14036386306537802543_1.darshan | less
```
