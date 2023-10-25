#
#
# Requires the following definitions:
#   BOOST_ROOT
#   CMAKE_INSTALL_PREFIX
# If using Clang, then the DPCPP_HOME environment variable is also needed.
# If using Intel oneAPI, you should have already run the setvars.bat script from your oneAPI installation.
#
# 
cmake_minimum_required(VERSION 3.25.0)

project (sycl_test_lib)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(STATUS "Resetting shared library macro...")
  message(STATUS "Was: ${CMAKE_CXX_CREATE_SHARED_LIBRARY}")
  set(CMAKE_CXX_CREATE_SHARED_LIBRARY "${_CMAKE_VS_LINK_DLL}<CMAKE_CXX_COMPILER> ${CMAKE_CL_NOLOGO} <CMAKE_CXX_LINK_FLAGS> <OBJECTS> ${CMAKE_START_TEMP_FILE} -LD /link <LINK_FLAGS> <LINK_LIBRARIES> /out:<TARGET> /implib:<TARGET_IMPLIB> /pdb:<TARGET_PDB> /version:<TARGET_VERSION_MAJOR>.<TARGET_VERSION_MINOR>${_PLATFORM_LINK_FLAGS} ${CMAKE_END_TEMP_FILE}")
endif()
message(STATUS "Current value: ${CMAKE_CXX_CREATE_SHARED_LIBRARY}")

# Required for Clang (prevents trying to load the standard library as modules, which Clang doesn't like)
set(CMAKE_CXX_STANDARD 20)

# Build a list of source files.
# Add any new ones here as modules are added.
include(${sycl_test_lib_SOURCE_DIR}/src/sources.cmake)
set(SOURCES
	${SYCL_TEST_LIB_SOURCES}
)

# Build a library
add_library(sycl_test_lib SHARED ${SOURCES})
set_target_properties(sycl_test_lib PROPERTIES DEBUG_POSTFIX "_d")

# SYCL-specific stuff
if (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
 find_package(IntelSYCL REQUIRED)
 add_sycl_to_target(TARGET sycl_test_lib SOURCES ${SOURCES})
else (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 find_package(Clang REQUIRED)
 target_compile_options(sycl_test_lib PUBLIC
    -std:c++20 -v
    -fsycl -fsycl-targets=nvptx64-nvidia-cuda,spir64-unknown-unknown)
    #-Xsycl-target-backend=nvptx64-nvidia-cuda --cuda-gpu-arch=sm_75)
 target_link_directories(sycl_test_lib PRIVATE $ENV{DPCPP_HOME}/llvm/build/lib)
 set(CMAKE_CXX_LINK_FLAGS "-v -fsycl -fsycl-targets=nvptx64-nvidia-cuda,spir64-unknown-unknown")
 #set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -g /WX")
 set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} /WX")
 #set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Xsycl-target-backend=nvptx64-nvidia-cuda --cuda-gpu-arch=sm_75")
endif()

# Setup Boost Path Variables.
add_definitions( -DBOOST_ALL_NO_LIB -DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS filesystem system REQUIRED)

# Include directories.
include_directories(
	${Boost_INCLUDE_DIRS}
)

target_include_directories(sycl_test_lib PRIVATE .)

target_link_libraries(sycl_test_lib
   ${Boost_LIBRARIES}
)

set_target_properties(
  sycl_test_lib  PROPERTIES 
      COMPILE_FLAGS 
          ""
      COMPILE_FLAGS_DEBUG
          "/W3 /Zi /fp:precise /Gd /GR /EHsc /nologo"
      COMPILE_FLAGS_RELEASE
          "/MD /O2 /Ob2 /GS /GL /W3 /Gy /Gm- /Zc:wchar_t /Zi /fp:precise /WX- /Gd /Oi /GR /MD /EHsc /nologo"
      LINK_FLAGS
		  ""
	  LINK_FLAGS_DEBUG
		  "/MANIFEST /LTCG /NXCOMPAT /SUBSYSTEM:WINDOWS /DLL /TLBID:1"
	  LINK_FLAGS_RELEASE
		  "/MANIFEST /LTCG /DYNAMICBASE /NXCOMPAT /SUBSYSTEM:WINDOWS /DLL /OPT:REF /OPT:ICF /TLBID:1"
)

install(TARGETS sycl_test_lib RUNTIME DESTINATION bin)
install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION bin OPTIONAL)
