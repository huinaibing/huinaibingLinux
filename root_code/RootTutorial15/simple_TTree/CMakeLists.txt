
cmake_minimum_required(VERSION 3.5)


project (root_ttree)


# 以下两行为需要用root的时候添加，下面link lib同理
find_package(ROOT REQUIRED COMPONENTS Core Tree RIO Net Hist MathCore)
include(${ROOT_USE_FILE})
# end

set(SOURCES
    src/Ana_mass.cpp
    src/Read_file.cpp
    src/run_all.cpp
)

add_executable(root_ttree ${SOURCES})

target_include_directories(root_ttree
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

# 需要root的时候添加
target_link_libraries(root_ttree
    PRIVATE
        ROOT::Core
        ROOT::Tree
        ROOT::RIO
        ROOT::Net
        ROOT::Hist
        ROOT::MathCore
)
# end