cmake_minimum_required(VERSION 3.5)

project(root_tree)

set(SOURCES
    src/run_all.cpp
    src/Read_file.cpp
    src/Ana_mass.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)