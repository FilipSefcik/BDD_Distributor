#include "MPICommunicator.h"

void MPICommunicator::sendInt(int message, int recvRank) {
    MPI_Send(&message, 1, MPI_INT, recvRank, 0, MPI_COMM_WORLD);
}

int MPICommunicator::recvInt(int sendRank) {
    int message;
    MPI_Recv(&message, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void MPICommunicator::sendDouble(double message, int recvRank) {
    MPI_Send(&message, 1, MPI_DOUBLE, recvRank, 0, MPI_COMM_WORLD);
}

double MPICommunicator::recvDouble(int sendRank) {
    double message;
    MPI_Recv(&message, 1, MPI_DOUBLE, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void MPICommunicator::sendString(std::string message, int recvRank) {
    int size = message.size() + 1;
    MPI_Send(&size, 1, MPI_INT, recvRank, 0, MPI_COMM_WORLD);
    MPI_Send(message.c_str(), size, MPI_CHAR, recvRank, 0, MPI_COMM_WORLD);
}

std::string MPICommunicator::recvString(int sendRank) {
    int size;
    char* message;
    MPI_Recv(&size, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    message = (char*)malloc(sizeof(char)*size);
    MPI_Recv(message, size, MPI_CHAR, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}