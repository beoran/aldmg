cmake_minimum_required(VERSION 2.8)
# Set policy: link_directories is relative to source dir
cmake_policy(SET CMP0015 NEW)

# Set the project name.
project(DMGLFA C)

# Tell CMake to search first in the cmake subdirectory for FIND_PACKAGE() or INCLUDE().
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

include(CheckCSourceCompiles)

if(CMAKE_COMPILER_IS_GNUCC)
 set(COMPILER_GCC 1)
set(CMAKE_C_FLAGS "-Wall -Wno-unused -Wno-unknown-pragmas -Wstrict-aliasing -fno-strict-aliasing -g -std=gnu99 -ffast-math")  
# always use gnu99, debugging, all warnings except unused and unknown pragmas.
# when compiling with gnu compiler    
endif(CMAKE_COMPILER_IS_GNUCC)

# Find the needed packages.
find_package(Allegro50 REQUIRED)

# Set include and lib dirs. 
include_directories(${ALLEGRO_INCLUDE_DIR})

set(DMGLFA_LIBS ${LIBS} ${ALLEGRO_LIBRARIES})

include_directories("include")
include(DmglfaFiles)

# Alum test exes in the bin subdir (first one should work, but doesnt, hmmm...)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# Compile all files except main
add_library(DMGLFA_OBJECTS OBJECT ${DMGLFA_SRC_FILES})
# Test exectable
add_executable(dmglfa $<TARGET_OBJECTS:DMGLFA_OBJECTS> src/main.c)
target_link_libraries(dmglfa ${DMGLFA_LIBS})

set(DMGLFA_REFMAN_INDEX  ${CMAKE_CURRENT_SOURCE_DIR}/doc/refman/index.html)
set(DMGLFA_REFMAN  ${CMAKE_CURRENT_SOURCE_DIR}/doc/refman)
set(DMGLFA_PROJECT ${CMAKE_CURRENT_SOURCE_DIR}/doc/nd)
set(DMGLFA_ND_IN   ${CMAKE_CURRENT_SOURCE_DIR})
set(DMGLFA_ND /usr/bin/naturaldocs)

add_custom_target(docs
    ${DMGLFA_ND} -i ${DMGLFA_ND_IN} -p ${DMGLFA_ND_IN} -o HTML ${DMGLFA_REFMAN}
    DEPENDS ${DMGLFA_SRC_FILES}
    SOURCES ${DMGLFA_SRC_FILES}
    VERBATIM
  )

# output  ${ALUM_REFMAN_INDEX}
# Naturaldocs 
# add_custom_target(alum_docs ${ALUM_ND})

#  [args1...]]
#                     [COMMAND command2 [args2...] ...]
#                     [DEPENDS depend depend depend ... ]
#                     [WORKING_DIRECTORY dir]
#                     [COMMENT comment] [VERBATIM]
#                     [SOURCES src1 [src2...]]


