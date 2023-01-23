#!/bin/bash
#SBATCH -A snic2022-22-964
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 4
#SBATCH --time=00:05:00
#SBATCH --error=jobs/960x540-1000iter_pixeltask.err
#SBATCH --output=jobs/960x540-1000iter_pixeltask.out

export OMP_NUM_THREADS=4

srun bin/5/pixeltask.out < ./in/5/960x540-1000iter
