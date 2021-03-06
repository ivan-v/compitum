cmake_minimum_required(VERSION 3.10)

project(Compitum LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang-cl)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(compitum
    PRIVATE ${PROJECT_SOURCE_DIR})

add_executable(main
    compitum/apparel.cpp
    compitum/character.cpp
    compitum/combat.cpp
    compitum/config.cpp
    compitum/dice.cpp
    compitum/infrastructure.cpp
    compitum/interactor.cpp
    compitum/main.cpp
    compitum/population.cpp
    compitum/region.cpp
    compitum/trade_good.cpp
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
