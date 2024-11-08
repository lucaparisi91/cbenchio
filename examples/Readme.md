# Profiling with Darshan

Load darshan and check in which folder the logs have been saved. 

```bash
module use /work/z19/shared/lparisi/courses/io/modules
module load darshan/3.4.6
```

Darshan is configured so that logs are saved in the folder defined in  `$DARSHAN_LOG_DIR_PATH`.

You can check which directories darshan look into when saving logs with the command `darshan-config --log-path`.

You can generate a summary report using the `darshan-parser` command. For instance

```bash
darshan-parser $DARSHAN_LOG_DIR_PATH/lparisi_benchio_id7999586-88755_11-8-32949-14036386306537802543_1.darshan | less
```

You can generate a trace  by using the `darshan-dxt-parser` command.

```bash
darshan-dxt-parser $DARSHAN_LOG_DIR_PATH/lparisi_benchio_id7999586-88755_11-8-32949-14036386306537802543_1.darshan | less
```