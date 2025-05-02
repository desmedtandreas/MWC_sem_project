#!/bin/bash

# Setting the control parameters of the Slurm scheduler
#SBATCH --job-name=run_seq
#SBATCH --output="%x-%J.out"
#SBATCH --error="%x-%J.err"

# Activation of HPE CPE
source /etc/profile.d/zz-cray-pe.sh

# Setting environment variables for the scheduled task
#module load cray-mvapich2_pmix_nogpu/2.3.7
module load cray-mvapich2_pmix_nogpu

# After the srun command, write the path to your program and its arguments for running on the scheduled computing nodes:
srun ./build/program.out $1

exit 0