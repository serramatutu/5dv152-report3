#!/bin/bash

for JOBSCRIPT in jobscripts/*; do
    sbatch $JOBSCRIPT
done
