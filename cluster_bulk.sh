#!/bin/bash
#SBATCH --job-name=mincut_grid
#SBATCH --array=1-75
#SBATCH --output="slurm-%A_%a.out"
#SBATCH --error="slurm-%A_%a.err"

# Load environment
module load cray-mvapich2_pmix_nogpu
source /etc/profile.d/zz-cray-pe.sh

# Read parameters from the file
CONFIG=$(sed -n "${SLURM_ARRAY_TASK_ID}p" configurations.txt)
read INSTANCE NODES CORES NSTATES <<< "$CONFIG"

# Set the correct resources dynamically
#SBATCH --nodes=1           # Dummy; we'll override below

# Run the actual job using srun with dynamic params
srun --nodes=$NODES --cpus-per-task=$CORES ./build/mpi_program "$INSTANCE" "$NSTATES" "$CORES"