#!/bin/bash

# Should be used only in build directory!!!!!! This is here only because of github!!!!!

# Prompt user for number of processes
read -p "Enter the number of processes: " num_processes

# Prompt user for path to config file
read -p "Enter the path to the config file: " conf_path

# Prompt user for number of dividers
read -p "Enter the number of divider [var - 0 / node - 1]: " num_divider

read -p "Enter which state you want availability to be calculated [0/1]: " state

# Construct mpiexec command
mpi_command="mpiexec -n $num_processes BDD_Distributor/main $conf_path $num_divider $state"

# Print the constructed command
echo "Executing command: $mpi_command"

# Execute the mpiexec command
$mpi_command