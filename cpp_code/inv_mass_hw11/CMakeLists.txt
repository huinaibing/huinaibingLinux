cmake_minimum_required(VERSION 3.5)

project(calcu_inv_mass_hw11)

set(SOURCES
    /home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/src/cal_inv_mass_.cpp
    src/main.cpp
    src/inv_mass_overload.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        /home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/include
)