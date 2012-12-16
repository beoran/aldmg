cmake_minimum_required(VERSION 2.8)
# Set policy: link_directories is relative to source dir
cmake_policy(SET CMP0015 NEW)

# Set the project name.
project(ALUM C)

# Tell CMake to search first in the cmake subdirectory for FIND_PACKAGE() or INCLUDE().
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)

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

set(ALUM_LIBS ${LIBS} ${ALLEGRO_LIBRARIES})

include_directories("include")
include(AlumFiles)

# Alum test exes in the bin subdir (first one should work, but doesnt, hmmm...)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# Compile all files except main
add_library(ALUM_OBJECTS OBJECT ${ALUM_SRC_FILES})
# Alps test exectable
add_executable(alum $<TARGET_OBJECTS:ALUM_OBJECTS> src/main.c)
target_link_libraries(alum ${ALUM_LIBS})


# Naturaldocs 
add_custom_target(alum_docs Name [ALL] [command1 [args1...]]
                    [COMMAND command2 [args2...] ...]
                    [DEPENDS depend depend depend ... ]
                    [WORKING_DIRECTORY dir]
                    [COMMENT comment] [VERBATIM]
                    [SOURCES src1 [src2...]])

set(ALUM_REFMAN_INDEX  ${CMAKE_CURRENT_SOURCE_DIR}/doc/refman/index.html)
set(ALUM_REFMAN  ${CMAKE_CURRENT_SOURCE_DIR}/doc/refman)
set(ALUM_PROJECT ${CMAKE_CURRENT_SOURCE_DIR}/doc/nd)
set(ALUM_ND_IN   ${CMAKE_CURRENT_SOURCE_DIR})
set(ALUM_ND /opt/naturaldocs/NaturalDocs)

add_custom_command(
    OUTPUT ${ALUM_REFMAN_INDEX}
    COMMAND ${ALUM_ND} -i ${ALUM_ND_IN} -p ${ALUM_ND_IN} -o HTML ${ALUM_REFMAN}
    DEPENDS ${ALUM_SRC_FILES}
  )
