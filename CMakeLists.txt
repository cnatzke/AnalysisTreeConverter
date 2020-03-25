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
set(GRSI_INCLUDE_DIRS $ENV{GRSISYS}/include $ENV{GRSISYS}/GRSIData/include)
set(GRSI_LINK_LIBS $ENV{GRSISYS}/lib $ENV{GRSISYS}/GRSIData/lib)

#set(GRSI_LIBS ${GRSISYS}/lib ${GRSISYS}/GRSIData/lib) # -L'

#Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT CONFIG REQUIRED)
include(${ROOT_USE_FILE})

# Adding src files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
#file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(TreeConverter ${SOURCES})

# linking libraries
#target_link_libraries(TreeConverter PRIVATE
#    ${GRSI_LINK_LIBS}
#    Spectrum
#    Minuit
#    PyROOT
#)

# add the binary tree to the search path for include files so that we will find AnalysisTreeConverter.h
target_include_directories(TreeConverter PRIVATE
    "${PROJECT_BINARY_DIR}"
    ${GRSI_INCLUDE_DIRS}
    include
    )


# add install targets
install(TARGETS TreeConverter DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/AnalysisTreeConverter.h"
    DESTINATION include
)
