#
#
# Requires the following definitions:
#   BOOST_ROOT
#   CMAKE_INSTALL_PREFIX
#
# 
cmake_minimum_required(VERSION 3.25.0)

project (sycl_test_offload)

# Setup Boost Path Variables.
add_definitions( -DBOOST_ALL_NO_LIB -DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS filesystem system REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/lib/src")

file(TO_CMAKE_PATH "${CMAKE_INSTALL_PREFIX}/bin" SYCL_TEST_LIB_DIR)
message(STATUS "SYCL_TEST_LIB_DIR: ${SYCL_TEST_LIB_DIR}")

# Include directories.
include_directories(
   ${Boost_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/lib/src
)

# Build a list of source files.
# Add any new ones here as modules are added.
set(SOURCES
   ${sycl_test_offload_SOURCE_DIR}/src/sycl_test_offload.cpp
)

# Build an executable
add_executable(sycl_test_offload ${SOURCES} )

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 find_package(Clang REQUIRED)
 target_compile_options(sycl_test_offload PRIVATE -std:c++20)
else (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
 target_compile_options(sycl_test_offload PRIVATE /std:c++20 /EHsc)
endif()

# Link these libraries.
target_link_libraries(sycl_test_offload
   ${Boost_LIBRARIES}
)

install(TARGETS sycl_test_offload RUNTIME DESTINATION bin)
install(FILES $<TARGET_PDB_FILE:sycl_test_offload> DESTINATION bin OPTIONAL)