#pragma once
#include <string>
#include <mpi.h>

class mpi_communicator
{
public:
    void send_int(int message, int recvievers_rank);
    int recv_int(int sender_rank);

    void send_double(double message, int recvievers_rank);
    double recv_double(int sender_rank);

    void send_string(std::string message, int recvievers_rank);
    std::string recv_string(int sender_rank);
};
