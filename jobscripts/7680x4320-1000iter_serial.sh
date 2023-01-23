#!/bin/bash
#SBATCH -A snic2022-22-964
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 4
#SBATCH --time=00:05:00
#SBATCH --error=jobs/7680x4320-1000iter_serial.err
#SBATCH --output=jobs/7680x4320-1000iter_serial.out

export OMP_NUM_THREADS=4

srun bin/5/serial.out < ./in/5/7680x4320-1000iter
