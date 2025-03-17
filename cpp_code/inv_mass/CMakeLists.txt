cmake_minimum_required(VERSION 3.5)

project(calcu_inv)

set(SOURCES
    src/cal_inv_mass.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)