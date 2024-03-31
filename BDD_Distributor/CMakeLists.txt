add_executable(
    main
        main.cpp
        module.cpp
        module_manager.cpp
        mpi_manager.cpp
        mpi_communicator.cpp
        bdd_distributor.cpp
)

find_package(TeDDy REQUIRED)

target_link_libraries(
    main PRIVATE teddy::teddy
)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(
    main PUBLIC MPI::MPI_CXX
)