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

# Function to validate char input
validate_char_input() {
    local input=$1
    local param1=$2
    local param2=$3

    # Convert input to lowercase
    input=$(echo "$input" | tr '[:upper:]' '[:lower:]')

    # Check if input matches param1 or param2
    while [[ $input != "$param1" && $input != "$param2" ]]; do
        read -p "Invalid input. Please enter '$param1' or '$param2': " input
        input=$(echo "$input" | tr '[:upper:]' '[:lower:]')  # Convert to lowercase
    done

    echo "$input"
}

get_count() {
    local input=$1
    local count=0
    local command_switch=""

    if [[ $input == "t" ]]; then
        count=$(nproc --all)
        command_switch="--use-hwthread-cpus"
    elif [[ $input == "c" ]]; then
        count=$(grep -m 1 'cpu cores' /proc/cpuinfo | awk '{print $4}')
    else
        echo "Invalid input"
        return 1
    fi

    echo "$count $command_switch"
}

# Usage: validate_char_input "$input" "y" "n" for 'y' or 'n'

read -p "Do you want to use a threads or cores? (t/C): " thread_or_core
thread_or_core=$(validate_char_input "$thread_or_core" "t" "c")

result=$(get_count "$thread_or_core")
available_count=$(echo "$result" | awk '{print $1}')
command_switch=$(echo "$result" | awk '{print $2}')

# Prompt user for number of processes
read -p "Enter the number of processes: " num_processes
num_processes=$(validate_input "$num_processes" "$available_count")

# Prompt user for path to config file
read -p "Enter the path to the config file: " conf_path

# Prompt user for number of dividers
read -p "Enter the number of divider [var - 0 / node - 1]: " num_divider
num_divider=$(validate_input "$num_divider" 1)

read -p "Enter which state you want availability to be calculated [0/1]: " state
state=$(validate_input "$state" 1)

# Prompt user if they want to use a timer
read -p "Do you want to use a timer? (y/N): " use_timer
use_timer=$(validate_char_input "$use_timer" "y" "n")

# Construct mpiexec command
mpi_command="mpiexec $command_switch -n $num_processes BDD_Distributor/main $conf_path $num_divider $state $use_timer"

# pre použitie threadov použi --use-hwthread-cpus

# Print the constructed command
# echo "Executing command: $mpi_command"

# Execute the mpiexec command
$mpi_command
