#!/bin/bash

# Setting the control parameters of the Slurm scheduler
#SBATCH --job-name=run_mpi
#SBATCH --output="%x-%J.out"
#SBATCH --error="%x-%J.err"

# Activation of HPE CPE
source /etc/profile.d/zz-cray-pe.sh

# Setting environment variables for the scheduled task
#module load cray-mvapich2_pmix_nogpu/2.3.7
module load cray-mvapich2_pmix_nogpu

# Access the number of cores allocated per task (from the -c parameter)
CORES_PER_TASK=$SLURM_CPUS_PER_TASK
# Print the number of cores (for debugging/verification)

# After the srun command, write the path to your program and its arguments for running on the scheduled computing nodes:
srun ./build/mpi_program $1 $2 $CORES_PER_TASK

exit 0