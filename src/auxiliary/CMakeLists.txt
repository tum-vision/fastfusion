add_library(auxiliary ${CPU_LIBRARY_TYPE}
debug.cpp memory.cpp threadpool.cpp plywriter.cpp ocv_tools.cpp)
#cuda_add_library(auxiliaryGPU ${GPU_LIBRARY_TYPE}
#cuda_basic.cu
#OPTIONS ${CUDAOPTIONS})
