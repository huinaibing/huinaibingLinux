cmake_minimum_required(VERSION 3.5)

project(hw_13_root LANGUAGES CXX)

# 以下两行为需要用root的时候添加，下面link lib同理
find_package(ROOT REQUIRED COMPONENTS Core Tree RIO Net Hist MathCore Graf Physics)
include(${ROOT_USE_FILE})
# end


set(SOURCES
    /home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/src/cal_inv_mass_.cpp
    src/main.cpp
    src/inv_mass_root.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        /home/huinaibing/huinaibingLinux/cpp_code/inv_mass_2/include
)


# 需要root的时候添加
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ROOT::Core
        ROOT::Tree
        ROOT::RIO
        ROOT::Net
        ROOT::Hist
        ROOT::MathCore
        ROOT::Graf
        ROOT::Physics
)
# end