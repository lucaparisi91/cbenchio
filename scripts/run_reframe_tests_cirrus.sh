module load cray-python
module load reframe # ReFrame is the regression framework used to run regression tests
source cbenchio_env/bin/activate # Activate a Python virtual environment with h5py and other test dependencies

BENCHIO_ROOT=/work/z19/shared/lparisi/cbenchio # Root directory of the repository
TESTS_ROOT=$BENCHIO_ROOT/cbenchio/regression_tests
export PYTHONPATH=$BENCHIO_ROOT/cbenchio:$PYTHONPATH # Make available python scripts used by the regression tests
reframe -R -C  $TESTS_ROOT/platform/cirrus-ex.py --keep-stage-files -c $TESTS_ROOT -S benchio_exe="/work/z19/shared/lparisi/cbenchio/opt/cbenchio/0.1/bin/benchio" -r # Run the regression tests with ReFrame, using the benchio executable specified by the -S option on the Cirrus-ex platform.