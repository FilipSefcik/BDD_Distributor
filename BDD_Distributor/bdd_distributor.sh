#!/bin/bash

# Function to validate input
validate_input() {
    local input=$1
    local max_value=$2
    while [[ $input -lt 0 || $input -gt $max_value ]]; do
        read -p "Invalid input. Please enter a number between 0 and $max_value: " input
    done
    echo "$input"
}

# Function to validate yes/no input
validate_yes_no_input() {
    local input=$1
    input=$(echo "$input" | tr '[:upper:]' '[:lower:]')  # Convert to lowercase
    while [[ ! $input =~ ^[yn]$ ]]; do
        read -p "Invalid input. Please enter 'y' or 'n': " input
        input=$(echo "$input" | tr '[:upper:]' '[:lower:]')  # Convert to lowercase
    done
    echo "$input"
}

# Prompt user for number of processes
read -p "Enter the number of processes: " num_processes
num_processes=$(validate_input "$num_processes" 16)

# Prompt user for path to config file
read -p "Enter the path to the config file: " conf_path

# Prompt user for number of dividers
read -p "Enter the number of divider [var - 0 / node - 1]: " num_divider
num_divider=$(validate_input "$num_divider" 1)

read -p "Enter which state you want availability to be calculated [0/1]: " state
state=$(validate_input "$state" 1)

# Prompt user if they want to use a timer
read -p "Do you want to use a timer? (y/N): " use_timer
use_timer=$(validate_yes_no_input "$use_timer")

# Construct mpiexec command
mpi_command="mpiexec -n $num_processes BDD_Distributor/main $conf_path $num_divider $state $use_timer"

# Print the constructed command
# echo "Executing command: $mpi_command"

# Execute the mpiexec command
$mpi_command
