# Ant Colony Optimization
 A program designed to find optimal path for e-learning system using ant colony optimization. <br>
 
Part of coursework for Soft Computing under the guidance of Dr. Shampa Chakraverty <br><br>

A program designed to find optimal path for e-learning system : <br>
The following are given about the system : <br>
a) Credits of a course <br>
b) Time required to complete a course <br>
c) Difficulty[i][j] representing the difficulty of other course j given learner has completed course i <br>

## Objectives
The following objectives needs to be fulfilled : <br>
a) The credits along a path must be maximized. <br>
b) The total difficulty of the path must be minimized. <br>

## Constraints
The following are the constraints : <br>
a) Minimum credits that the user wants to acquire <br>
b) The total time limit must not exceed the maximum time limit <br>

## Modelling of the problem
The problem is modelled by a graph as follows: <br>
a) The nodes of the graph are course modules. They have 2 properties : credits and time_required <br>
b) The edges of the graph contain the difficulty of going from a course i to a course j <br>
c) Pherome matrix : To store pherome values. <br>
d) pherome_increment_prob : A matrix to store pherome Increments of various ants. <br>

# Run on a linux system with min gcc version of 4.2.0
## To check if OPENMP is installed, run
echo | cpp -fopenmp -dM | grep -i open

The command should display something like : <br>
#define _OPENMP 201511 <br>
201511 means : year 2015, month November(11) 

# Running Instructions
<b> To specify the number of parallel threads for open mp </b> <br>
export OMP_NUM_THREADS=<omp_num_threads> <br>
<b> Compile the code </b> <br>
g++ -std=c++11 AntColonyOptimization.cpp -fopenmp -o aco <br>
<b> Run the code </b> <br>
./aco <number_of_ants> <br>
