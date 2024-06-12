add_executable(
    bdd_parallel
        main.cpp
        module.cpp
        module_manager.cpp
        mpi_manager.cpp
        mpi_communicator.cpp
        bdd_distributor.cpp
        merger.cpp
        pla_function.cpp
)

find_package(TeDDy REQUIRED)

target_link_libraries(
    bdd_parallel PRIVATE teddy::teddy
)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(
    bdd_parallel PUBLIC MPI::MPI_CXX
)