set -e
set -x

module load PrgEnv-gnu
module load cray-hdf5-parallel
source settings_archer2.sh



SOURCE_DIR=$(pwd)/../cbenchio

rm -rf build
mkdir build 
cd build 
CXX=CC CC=cc FC=ftn cmake $SOURCE_DIR -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX/cbenchio/0.1
make -j 4
make install
