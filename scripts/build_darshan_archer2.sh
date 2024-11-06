set -x 
set -e

source settings_archer2.sh

DARSHAN_VERSION=3.4.6

DARSHAN_ROOT=$INSTALL_PREFIX/darshan/$DARSHAN_VERSION

rm -fr $DARSHAN_ROOT

module restore
module load PrgEnv-gnu
cd src
wget https://web.cels.anl.gov/projects/darshan/releases/darshan-$DARSHAN_VERSION.tar.gz 
tar -zxvf darshan-$DARSHAN_VERSION.tar.gz

cd darshan-$DARSHAN_VERSION
./prepare.sh 
cd darshan-runtime

export DARSHAN_LOG_DIR_PATH=${DARSHAN_ROOT}/darshan-logs

CC=mpicc ./configure --prefix=${DARSHAN_ROOT} --with-log-path-by-env=DARSHAN_LOG_DIR_PATH  --with-log-path=$DARSHAN_LOG_DIR_PATH --with-jobid-env=SLURM_JOB_ID 
make
make install

# Install utils for visualization. Genarating reports require pdflatex atc.. 

cd ../darshan-util
./configure --prefix=${DARSHAN_ROOT}
make 
make install

# Creates directory structures for the log
cd ${DARSHAN_ROOT}/bin
mkdir -p $DARSHAN_LOG_DIR_PATH
./darshan-mk-log-dirs.pl