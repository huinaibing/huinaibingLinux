cmake_minimum_required(VERSION 3.5)

project(hw10)

set(SOURCES
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)