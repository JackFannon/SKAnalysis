#!/bin/bash

#Application path location of applicaiton

Dependencies=`pwd`/Dependencies

source ../SKG4/G4ROOTsource.sh

export LD_LIBRARY_PATH=`pwd`/lib:${Dependencies}/relic_sk4_ana/data_reduc/third/lib:${Dependencies}/TMVA/lib:$LD_LIBRARY_PATH

for folder in `ls -d ${PWD}/UserTools/*/ `
do
    export PYTHONPATH=$folder:${PYTHONPATH}
done

export SEGFAULT_SIGNALS="all"

# new location of secret sauce on new sukap
export RFA_ROOT='/opt/FJSVrdass/lib'
