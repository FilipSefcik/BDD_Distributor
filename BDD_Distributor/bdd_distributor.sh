#!/bin/bash

export PMIX_MCA_pcompress_base_silence_warning=1

# Function to validate input
validate_input() {
    local input=$1
    local min_value=$2
    local max_value=$3
    while [[ ! $input =~ ^[0-9]+$ || $input -lt min_value || $input -gt $max_value ]]; do
        read -p "Invalid input. Please enter a number between $min_value and $max_value: " input
    done
    echo "$input"
}

# Function to validate char input
# Usage: validate_char_input "$input" "y" "n" for 'y' or 'n'
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

# Function to get how many processed can be used depending on usage of cores or threads
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

# Function to ask for a file path and check if the file exists
get_file() {
    #local input=$1
    local file_path

    read -p "Enter the path to the $1: " file_path

    # Loop until a valid file path is provided
    while [ ! -f "$file_path" ]; do
        read -p "Path to $file_path does not exist. Enter the path to the $1: " file_path
    done
    
    echo "$file_path"
}

path_to_main="bdd_parallel"

# Check if the file bdd_parallel.exe exists in the current directory
if [ ! -f "$path_to_main" ] && [ ! -f "$path_to_main.exe" ]; then
    echo "bdd_parallel.exe not found in the current directory."
    path_to_main=$(get_file "bdd_parallel.exe")
fi

# Prompt user for usage of threads or cores
read -p "Do you want to use a threads or cores? (t/C): " thread_or_core
thread_or_core=$(validate_char_input "$thread_or_core" "t" "c")

result=$(get_count "$thread_or_core")
available_count=$(echo "$result" | awk '{print $1}')
command_switch=$(echo "$result" | awk '{print $2}')

# Prompt user for number of processes
read -p "Enter the number of processes: " num_processes
num_processes=$(validate_input "$num_processes" 1 "$available_count")

# Prompt user for path to config file
conf_path=$(get_file "config file")

# Prompt user for number of dividers
read -p "Enter the number of divider [var - 0 / node - 1]: " num_divider
num_divider=$(validate_input "$num_divider" 0 1)

# Prompt user for state which availability will be calculated
read -p "Enter which state you want availability to be calculated [0/1]: " state
state=$(validate_input "$state" 0 1)

# Prompt user if they want to use a timer
read -p "Do you want to use a timer? (y/N): " use_timer
use_timer=$(validate_char_input "$use_timer" "y" "n")

# Construct mpiexec command
mpi_command="mpiexec $command_switch -n $num_processes $path_to_main $conf_path $num_divider $state $use_timer"

# Execute the mpiexec command
$mpi_command
