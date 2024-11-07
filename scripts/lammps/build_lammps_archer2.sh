source ../settings_archer2.sh

module load PrgEnv-gnu
module load cray-fftw
module load cmake
module load eigen
module load cray-python
module load cray-hdf5-parallel
module load cray-parallel-netcdf

set -e 
set -x

source ../settings_archer2.sh 

LAMMPS_NAME=lammps-2024-02-13
LAMMPS_ROOT=$INSTALL_PREFIX/lammps/$LAMMPS_NAME
LAMMPS_SOURCE=$(pwd)/$LAMMPS_NAME


rm -rf $LAMMPS_SOURCE
git clone --depth=1 --branch patch_7Feb2024_update1 https://github.com/lammps/lammps.git $LAMMPS_NAME

rm -rf build
mkdir -p build
cd build

cmake -C $LAMMPS_SOURCE/cmake/presets/most.cmake\
      -D BUILD_MPI=on\
      -D BUILD_SHARED_LIBS=yes \
      -D CMAKE_CXX_COMPILER=CC \
      -D CMAKE_CXX_FLAGS="-O2" \
      -D CMAKE_INSTALL_PREFIX=$LAMMPS_ROOT  \
      -D EIGEN3_INCLUDE_DIR=/work/y07/shared/libs/core/eigen/3.4.0/include \
      -D FFT=FFTW3 \
      -D FFTW3_INCLUDE_DIR=${FFTW_INC} \
      -D FFTW3_LIBRARY=${FFTW_DIR}/libfftw3_mpi.so \
      -D PKG_H5MD=yes \
      -D PKG_NETCDF=yes \
      $LAMMPS_SOURCE/cmake/

VERBOSE=1 make -j 16 
make install