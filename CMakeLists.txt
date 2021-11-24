#________________________________________________________________
#
# FullC0ntact V2 Library : Top-Level CMakeLists file
# Author                 : Raphael Muenster
#________________________________________________________________
#
# cmake version
cmake_minimum_required(VERSION 3.2)
CMAKE_POLICY(SET CMP0054 NEW)
CMAKE_POLICY(SET CMP0074 NEW)
 
# name of the project
PROJECT(MPI-Test-Suite)

# allow easier if-else syntax
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)


#==================================================================================================
# MPI Setup
#==================================================================================================
FIND_PACKAGE( MPI REQUIRED)

IF( MPI_FOUND )
   SET( HAVE_MPI BOOL ON )
   add_compile_definitions(HAVE_MPI=1)
ELSE()
   SET( HAVE_MPI BOOL OFF )
ENDIF( MPI_FOUND )


add_executable(example1 example1.cpp)
target_include_directories(example1 PUBLIC ${MPI_CXX_HEADER_DIR})
target_link_libraries(example1 ${MPI_msmpi_LIBRARY})

add_executable(example2 example2.cpp)
target_include_directories(example2 PUBLIC ${MPI_CXX_HEADER_DIR})
target_link_libraries(example2 ${MPI_msmpi_LIBRARY})

add_executable(example3 example3.cpp)
target_include_directories(example3 PUBLIC ${MPI_CXX_HEADER_DIR})
target_link_libraries(example3 ${MPI_msmpi_LIBRARY})


