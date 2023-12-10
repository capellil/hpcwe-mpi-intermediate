#!/bin/bash

#SBATCH --account=pawsey0007
#SBATCH -p work
#SBATCH --time=00:00:20
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH -o output.txt

srun ./bin/preamble