#!/bin/bash
#SBATCH --job-name=INF503HW6                  
#SBATCH --output=results.out
#SBATCH --error=results.err
#SBATCH --time=20:00          	   
#SBATCH --mem=100000
#SBATCH -c1
#SBATCH -C bw

module load gcc

#run makefile first
make


srun ./homework Problem1A 5000

srun ./homework Problem1B 5000
srun ./homework Problem1B 50000
srun ./homework Problem1B 100000
srun ./homework Problem1B 1000000
srun ./homework Problem1B 5000000
srun ./homework Problem1B 10000000
srun ./homework Problem1B 20000000
srun ./homework Problem1B 50000000

srun ./homework Problem2A 5000

srun ./homework Problem2B 5000
srun ./homework Problem2B 50000
srun ./homework Problem2B 100000
srun ./homework Problem2B 1000000
srun ./homework Problem2B 5000000
srun ./homework Problem2B 10000000
srun ./homework Problem2B 20000000
srun ./homework Problem2B 50000000