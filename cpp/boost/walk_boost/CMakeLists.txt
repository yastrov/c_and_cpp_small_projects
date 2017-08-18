cmake_minimum_required (VERSION 2.8.12.2 FATAL_ERROR)

set (PROJECT_NAME "walk_boost" CXX)

project (${PROJECT_NAME})
set (PROJECT_SOURCE_DECLARATION_DIRECTORY ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DEFINITION_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set (MAIN_FILE ${PROJECT_SOURCE_DEFINITION_DIRECTORY}/main.cpp)
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions("-std=c++11")

IF (WIN32)
  #set (CMAKE_MAKE_PROGRAM "C:/MinGW_nuwen64/bin/make.exe")
  #set (CMAKE_C_COMPILER "C:/MinGW_nuwen64/bin/gcc.exe")
  #set (CMAKE_CXX_COMPILER "C:/MinGW_nuwen64/bin/g++.exe")
  set (MINGW_FOLDER "C:/MinGW_nuwen64")
  #set (CMAKE_MAKE_PROGRAM "${MINGW_FOLDER}/bin/make.exe")
  set (CMAKE_MAKE_PROGRAM "${MINGW_FOLDER}/bin/mingw32-make.exe")
  set (CMAKE_C_COMPILER "${MINGW_FOLDER}/bin/gcc.exe")
  set (CMAKE_CXX_COMPILER "${MINGW_FOLDER}/bin/g++.exe")
ELSE()
  # set stuff for other systems
ENDIF()

IF (MINGW)
find_program(CMAKE_MAKE_PROGRAM
   NAMES make
         make.exe
   DOC "Find a suitable make program for building under Windows/MinGW"
   HINTS c:/MinGW-32/bin 
   PATHS ${MINGW_FOLDER})
ELSE()
ENDIF()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Set options used by "FindBoost.cmake".

# Either specify BOOST_ROOT or BOOST_INCLUDEDIR and BOOST_LIBRARYDIR.
# I am using "MinGW Distro" in this example, but also a relative path, e. g.
# "vendor/boost" can be specified.
#set (BOOST_ROOT "C:/MinGW_nuwen64")
set (Boost_DIR "C:/MinGW_nuwen64/include/boost")
set (BOOST_INCLUDEDIR "C:/MinGW_nuwen64/include/")
set (BOOST_LIBRARYDIR "C:/MinGW_nuwen64/lib")
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

# Load settings for the external project "Boost".
# The components "system" and "filesystem" from "Boost" version 1.55.0 or
# greater are required.
find_package (Boost 1.59.0 REQUIRED COMPONENTS system filesystem program_options)
if (Boost_FOUND)
  # From the offical documentation:
  # Add include directories to the build. [...] If the SYSTEM option is given,
  # the compiler will be told the directories are meant as system include
  # directories on some platforms (signalling this setting might achieve effects
  # such as the compiler skipping warnings [...])."
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})

  # From the offical documentation:
  # "Specify directories in which the linker will look for libraries. [...] Note
  # that this command is rarely necessary. Library locations returned by
  # find_package() and find_library() are absolute paths. Pass these absolute
  # library file paths directly to the target_link_libraries() command. CMake
  # will ensure the linker finds them."
  #link_directories (${Boost_LIBRARY_DIRS})
endif ()

include_directories (${PROJECT_SOURCE_DECLARATION_DIRECTORY})

# "Add an executable to the project using the specified source files."
add_executable (${PROJECT_NAME} ${MAIN_FILE})
#add_executable (${PROJECT_NAME} ${PROJECT_NAME})
# "Link a target to given libraries."
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
