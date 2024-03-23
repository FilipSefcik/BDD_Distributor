add_executable(
    main
        main.cpp
        Module.cpp
        ModuleManager.cpp
        # Node.cpp
        MPIManager.cpp
        MPICommunicator.cpp
)

find_package(TeDDy REQUIRED)

target_link_libraries(
    main PRIVATE teddy::teddy
)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(main PUBLIC MPI::MPI_CXX)