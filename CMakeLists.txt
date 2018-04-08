cmake_minimum_required(VERSION 3.9)

project(Compitum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(compitum
    PRIVATE ${PROJECT_SOURCE_DIR})

add_executable(main
    infrastructure.cpp
    interactor.cpp
    main.cpp
    population.cpp
    region.cpp
    trade_good.cpp
    )

if(MSVC)
    target_compile_options(main
        PRIVATE
            /EHsc
    )
else()
    target_compile_options(main
        PRIVATE
            -Wall               # "all" warnings
            -Wextra             # extra warnings
            -pedantic           # even more warnings
            -O                  # optimize
            -ftrapv)            # trap on integer overflow
endif()
