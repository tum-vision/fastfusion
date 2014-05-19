
include_directories("/usr/include/nvidia-current/cuda")
include_directories("/work/sdks/cudaversions/cuda50/cuda/include")
include_directories("/home/steinbrf/sdks/cudaversions/cuda50/cuda/include")
#find_package(Qt REQUIRED)

#INCLUDE(${QT_USE_FILE})
include_directories("/usr/include/qt4/QtXml")
include_directories("/usr/include/qt4")
include_directories("/usr/include/qt4/Qt")
include_directories("/usr/include/qt4/QtCore")
include_directories("/usr/include/qt4/QtXml")
include_directories("/usr/include/qt4/QtGui")
include_directories("/usr/include/qt4/QtOpenGL")
ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories("..")

add_library(geometryfusion_aos STATIC
geometryfusion_aos.cpp
treeandbrick.cpp
treeandbrick_indexed.cpp
treeandbrick_incremental.cpp
treeandbrick_incremental_recursive.cpp
mesh.cpp
#meshcelltraversal.cpp
mesh_interleaved.cpp
mesh_interleaved_meshcell.cpp
)

add_library(geometryfusion_mipmap_cpu STATIC
geometryfusion_mipmap_cpu.cpp
loopclosure.cpp
)

