cmake_minimum_required(VERSION 3.5)

project(askGrade)

set(SOURCES
    src/ask_grade.cpp
    src/main.cpp
)

add_executable(askGrade ${SOURCES})

target_include_directories(askGrade
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)