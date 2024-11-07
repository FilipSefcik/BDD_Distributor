#include "mpi_communicator.h"

void mpi_communicator::send_int(int message, int recievers_rank) {
    MPI_Send(&message, 1, MPI_INT, recievers_rank, 0, MPI_COMM_WORLD);
}

int mpi_communicator::recv_int(int sender_rank) {
    int message;
    MPI_Recv(&message, 1, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void mpi_communicator::send_double(double message, int recievers_rank) {
    MPI_Send(&message, 1, MPI_DOUBLE, recievers_rank, 0, MPI_COMM_WORLD);
}

double mpi_communicator::recv_double(int sender_rank) {
    double message;
    MPI_Recv(&message, 1, MPI_DOUBLE, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void mpi_communicator::send_string(std::string message, int recievers_rank) {
    int size = message.size() + 1;
    MPI_Send(&size, 1, MPI_INT, recievers_rank, 0, MPI_COMM_WORLD);
    MPI_Send(message.c_str(), size, MPI_CHAR, recievers_rank, 0, MPI_COMM_WORLD);
}

std::string mpi_communicator::recv_string(int sender_rank) {
    int size;
    // char* message;
    MPI_Recv(&size, 1, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // message = (char*)malloc(sizeof(char)*size);
    char message[size];
    MPI_Recv(message, size, MPI_CHAR, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void mpi_communicator::scatter_ints(int* message, int* recieve_buffer) {
    MPI_Scatter(message, 1, MPI_INT, recieve_buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void mpi_communicator::gather_doubles(double* message, double* recieve_buffer) {
    MPI_Gather(message, 1, MPI_DOUBLE, recieve_buffer, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}