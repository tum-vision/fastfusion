add_subdirectory(camerautils)
add_subdirectory(auxiliary)
#add_subdirectory(auxiliaryGPU)
#add_subdirectory(libelas)
#add_subdirectory(output)
add_subdirectory(fusion)
#add_subdirectory(fusionGPU)
#add_subdirectory(sophus)
#add_subdirectory(filesystem) 


include_directories("/usr/include/qt4/QtXml")
include_directories("/usr/include/qt4")
include_directories("/usr/include/qt4/Qt")
include_directories("/usr/include/qt4/QtCore")
include_directories("/usr/include/qt4/QtXml")
include_directories("/usr/include/qt4/QtGui")
include_directories("/usr/include/qt4/QtOpenGL")
ADD_DEFINITIONS(${QT_DEFINITIONS})

if(EXISTS "${ROOT}/usr/lib/x86_64-linux-gnu/libQGLViewer.so")
message(STATUS "Found qglviewer2, linking QGLViewer")
set(QGLVIEWER QGLViewer)
else()
message(STATUS "Did not find qglviewer2, linking qglviewer-qt4")
set(QGLVIEWER qglviewer-qt4)
endif()


# add_executable(cameracalibration cameracalib_main.cpp)
# target_link_libraries(cameracalibration 
# opencv_core opencv_imgproc opencv_highgui opencv_features2d opencv_calib3d
# )
# 
# add_executable(rectify_known rectify_stereo_known_main.cpp)
# target_link_libraries(rectify_known 
# opencv_core opencv_imgproc opencv_highgui opencv_features2d opencv_calib3d
# )


FIND_PACKAGE(Qt4 REQUIRED)
QT4_WRAP_CPP(onlinefusion_HEADERS_MOC onlinefusionviewer.hpp)

add_executable(onlinefusion
onlinefusionviewer_main.cpp
onlinefusionviewer.cpp
${onlinefusion_HEADERS_MOC}
)
target_link_libraries(onlinefusion
geometryfusion_mipmap_cpu geometryfusion_aos
camerautils
auxiliary
${OpenCV_LIBS}
${QGLVIEWER}
QtCore QtGui QtOpenGL
GL GLU glut GLEW
boost_thread
boost_system
)