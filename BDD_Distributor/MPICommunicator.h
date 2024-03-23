#pragma once
#include <string>
#include <mpi.h>

class MPICommunicator
{
public:
    void sendInt(int message, int recvRank);
    int recvInt(int sendRank);

    void sendDouble(double message, int recvRank);
    double recvDouble(int sendRank);

    void sendString(std::string message, int recvRank);
    std::string recvString(int sendRank);

};
