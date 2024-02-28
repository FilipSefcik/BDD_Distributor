add_executable(
    main
        main.cpp
        Distributor.cpp
        Module.cpp
        ModuleManager.cpp
        Node.cpp
        PlaManager.cpp
)

find_package(TeDDy REQUIRED)

target_link_libraries(
    main PRIVATE teddy::teddy
)