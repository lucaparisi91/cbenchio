source env.sh
cd src
tar -zxvf darshan-3.4.4.tar.gz 
cd darshan-3.4.4
./prepare.sh 
cd darshan-runtime
install_dir=$(pwd)/../../../darshan-3.4.4
./configure --prefix=${install_dir} --with-log-path=${install_dir}/darshan-logs --with-jobid-env=SLURM_JOB_ID CC=mpicc
make
make install

# Install utils for visualization. Genarating reports require pdflatex atc.. 

cd ../darshan-util
./configure --prefix=${install_dir}
make 
make install

# Creates directory structures for the log
cd ${install_dir}
mkdir darshan-logs
cd bin
./darshan-mk-log-dirs.pl
