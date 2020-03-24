#------------------------------------------------------------------
#Setup the project
#------------------------------------------------------------------
cmake_minimum_required(VERSION 3.2)

project(TreeConverter VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# configure header file
configure_file(AnalysisTreeConverter.h.in AnalysisTreeConverter.h)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

# Include
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
include_directories(${GRSISYS}/include ${GRSISYS}/GRSIData/include) # -I's
link_directories(${GRSISYS}/lib ${GRSISYS}/GRSIData/lib) # -L's
find_package(ROOT CONFIG REQUIRED)

# Adding src files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(TreeConverter ${SOURCES})

# linking libraries
#target_link_libraries(
#    MathMore
#    Spectrum
#    Minuit
#    PyROOT
#)

# add the binary tree to the search path for include files so that we will find AnalysisTreeConverter.h
target_include_directories(TreeConverter PUBLIC
    "${PROJECT_BINARY_DIR}"
    include
    )

# add install targets
install(TARGETS TreeConverter DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/AnalysisTreeConverter.h"
    DESTINATION include
)
