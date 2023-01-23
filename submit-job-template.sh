#!/bin/bash
#SBATCH -A snic2022-22-964
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 4
#SBATCH --time=00:05:00
#SBATCH --error=jobs/{infile}_{impl}.err
#SBATCH --output=jobs/{infile}_{impl}.out

export OMP_NUM_THREADS=4

srun bin/5/{impl}.out < ./in/5/{infile}
