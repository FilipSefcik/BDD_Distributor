#pragma once
#include <mpi.h>
#include <string>

/*
 * @brief mpi_communicator simplifies usage of MPI message functions
 */
class mpi_communicator {
  public:
    static void send_int(int message, int recvievers_rank);
    static int recv_int(int sender_rank);

    static void send_double(double message, int recvievers_rank);
    static double recv_double(int sender_rank);

    static void send_string(std::string message, int recvievers_rank);
    static std::string recv_string(int sender_rank);

    static void scatter_ints(int* message, int* recieve_buffer);
    static void gather_doubles(double* message, double* recieve_buffer);
};
