add_executable(
    main
        main.cpp
        Distributor.cpp
        Module.cpp
        ModuleManager.cpp
        Node.cpp
        PlaManager.cpp
        MPIManager.cpp
)

find_package(TeDDy REQUIRED)

target_link_libraries(
    main PRIVATE teddy::teddy
)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(main PUBLIC MPI::MPI_CXX)