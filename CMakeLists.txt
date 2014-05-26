cmake_minimum_required(VERSION 2.8)
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_calib3d)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(ROS_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_BUILD_TYPE Debug)
#set(ROS_BUILD_TYPE Debug)

#FIND_PACKAGE(Qt4 REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(TBB)

#if(NOT TBB_FOUND)
#MESSAGE(STATUS "TBB not found!");
#else(NOT TBB_FOUND)
#include_directories(${TBB_INCLUDE_DIRS})
#link_directories(${TBB_LIBRARY_DIRS})
#endif(NOT TBB_FOUND)


if (CMAKE_COMPILER_IS_GNUCC)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

    message(STATUS "GCC Major Version is " ${GCC_MAJOR})
    message(STATUS "GCC Major Version is " ${GCC_MINOR})
endif()

set(CMAKE_C_FLAGS
-frounding-math 
#-Wall
)

execute_process(COMMAND cat /proc/cpuinfo OUTPUT_VARIABLE CPU_INFO)
string(REGEX MATCHALL "avx2" AVX_STRING ${CPU_INFO})
list(LENGTH AVX_STRING AVX_STRING_LENGTH)
# message(STATUS "AVX_STRING: " ${AVX_STRING})
message(STATUS "Number of avx2 occurrences in /proc/cpuinfo: " ${AVX_STRING_LENGTH})


if(${AVX_STRING_LENGTH} GREATER 0)
message(STATUS "Compiling with AVX2 support")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -mavx2 -funroll-loops -DOWNAVX")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -mavx2 -funroll-loops -DOWNAVX")
else()
message(STATUS "Compiling without AVX2 support")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -msse4.2 -funroll-loops")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -msse4.2 -funroll-loops")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPRINT_DEBUG")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPRINT_DEBUG")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)



include_directories(${PROJECT_SOURCE_DIR}/src)
message(STATUS "PROJECT_SOURCE_DIR is " ${PROJECT_SOURCE_DIR})

#set(Boost_DEBUG 0)
#set(BOOST_USE_STATIC_LIBS OFF)
#set(BOOST_USE_MULTITHREADED ON)
#set(BOOST_USE_STATIC_RUNTIME OFF)
#find_package(Boost COMPONENTS thread system tr1)
#include_directories(${Boost_INCLUDE_DIRS})

#find_package(Eigen3 REQUIRED)
#include_directories(EIGEN3_INCLUDE_DIR)
include_directories("/usr/include/eigen3")


## CUDA

#set(CUDA_TOOLKIT_ROOT_DIR "/work/sdks/cudaversions/cuda50/")
# find_package(CUDA 5.0 REQUIRED)
# if(CUDA_FOUND)
#     message("Found CUDA")
# else(CUDA_FOUND)
#     message("CUDA is not installed on this system.")
# endif()
# execute_process( COMMAND nvcc --version )
# include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include)
# set(TEST_CUDA_ARCH $ENV{CUDA_ARCH})
# if(TEST_CUDA_ARCH)
# message(STATUS "The GPU Architecture is set to $ENV{CUDA_ARCH} in an Environment Variable")
# set(CUDA_ARCH $ENV{CUDA_ARCH})
# else(TEST_CUDA_ARCH)
# message(STATUS "No GPU Architecture set, setting to sm_13")
# set(CUDA_ARCH sm_11)
# endif(TEST_CUDA_ARCH)
# #if(${CUDA_ARCH} matches sm_11)
# #set(CUDAARCHNUMBER 11)
# #elseif(${CUDA_ARCH} matches sm_13)
# #set(CUDAARCHNUMBER 13)
# #elseif(${CUDA_ARCH} matches sm_20)
# #set(CUDAARCHNUMBER 20)
# #endif(${CUDA_ARCH} matches sm_11)
# set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -arch=${CUDA_ARCH})






#include_directories(${PROJECT_SOURCE_DIR}/external)
#include_directories(${PROJECT_SOURCE_DIR}/external/math)
#include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/build/src)
#include_directories("/usr/include/nvidia-current/cuda")

# find_package(PCL 1.5 REQUIRED COMPONENTS common io filters)
# include_directories(${PCL_INCLUDE_DIRS})
# link_directories(${PCL_LIBRARY_DIRS})
# add_definitions(${PCL_DEFINITIONS})



#include(${QT_USE_FILE})
#add_definitions(${QT_DEFINITIONS})

# Doxygen integration
find_package(Doxygen)
if(DOXYGEN_FOUND)
#this adds the correct folder to INPUT= :
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

#add doc target
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "generating docs with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

add_subdirectory(src)

