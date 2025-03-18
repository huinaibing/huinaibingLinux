cmake_minimum_required(VERSION 3.5)

project(calcu_inv_mass)

set(SOURCES
    src/cal_inv_mass_.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)