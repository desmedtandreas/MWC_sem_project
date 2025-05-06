#!/bin/bash

# Setting the control parameters of the Slurm scheduler
#SBATCH --job-name=run_data
#SBATCH --output="%x-%J.out"
#SBATCH --error="%x-%J.err"

# Activation of HPE CPE
source /etc/profile.d/zz-cray-pe.sh

# Load necessary modules
module load cray-mvapich2_pmix_nogpu

# Access the number of cores allocated per task
CORES_PER_TASK=$SLURM_CPUS_PER_TASK

# Run the program
srun ./build/program $1 $CORES_PER_TASK

exit 0