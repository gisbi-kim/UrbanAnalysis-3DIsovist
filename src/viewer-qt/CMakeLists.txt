cmake_minimum_required(VERSION 2.8)

project(qtlas)

file(GLOB qt_path /opt/Qt*)

if(EXISTS ${qt_path})
  set(CMAKE_PREFIX_PATH ${qt_path}/5.6/gcc_64/lib/cmake)
  message("-- CMAKE_PREFIX_PATH is " ${qt_path}/5.6/gcc_64/lib/cmake)
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(OpenGL)
find_package(Qt5 QUIET COMPONENTS Core Xml OpenGL Gui Widgets)
find_package(PkgConfig REQUIRED)


### === las viewer part === ###


# INCLUDE LOCAL MODULES
#========================================================================
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "cmake")
file (GLOB MODULES "${CMAKE_MODULE_PATH}/*.cmake")
foreach (MODULE ${MODULES})
  include (${MODULE})
endforeach ()

# CONFIGURE C/C++
#========================================================================
set (CMAKE_C_CXX_COMMON_FLAGS "-Wall -Wfloat-equal -Wundef -Wno-strict-aliasing -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-macros")
set (CMAKE_C_FLAGS "-u MAIN__ -std=gnu99 -fopenmp ${CMAKE_C_CXX_COMMON_FLAGS}")
set (CMAKE_CXX_FLAGS "-u MAIN__ -std=gnu++11 -fopenmp ${CMAKE_C_CXX_COMMON_FLAGS}")

# CONFIG FILES
#========================================================================
set (CONFIG_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config)

#add_custom_target (config ALL
#  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/build/share
#  COMMAND ln -sf ${CONFIG_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/build/share/.
#)

install (DIRECTORY ${CONFIG_SOURCE_PATH}
  DESTINATION ${CMAKE_SOURCE_DIR}/build/share/.
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE
  DIRECTORY_PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE  GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE
  PATTERN ".svn" EXCLUDE
  )


# INCLUDE PROJECT SUBDIRECTORY
#========================================================================
#add_subdirectory(src)

libhandler_las ()



### === qt basic part === ###

set (QtRENDERING3D_CPP mainwindow.cpp openglwidget.cpp visualizer.cpp las_reader.cpp container.cpp)
set (QtRENDERING3D_HEADER mainwindow.h openglwidget.h visualizer.h las_reader.h container.h)
set (QtRENDERING3D_UI  mainwindow.ui)
set (QtRENDERING3D_MAIN main.cpp)

if(Qt5_FOUND)
  QT5_WRAP_UI(UI_HEADERS ${QtRENDERING3D_UI})
  QT5_WRAP_CPP(UI_SOURCES ${QtRENDERING3D_HEADER})
  QT5_ADD_RESOURCES(SHADER_RSC_ADDED resources.qrc)

  include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS}
)
  set(QtRENDERING3D_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES}
)
endif()

add_library(qtlasviewer_library 
  ${QtRENDERING3D_HEADER} ${QtRENDERING3D_CPP}
  ${UI_HEADERS} ${UI_SOURCES}
)

add_executable(qtlasviewer ${QtRENDERING3D_MAIN}  ${SHADER_RSC_ADDED})

target_link_libraries(qtlasviewer_library ${QGLVIEWER_LIBRARY} ${QtRENDERING3D_LIBRARIES} ${OPENGL_LIBRARY} ${IRPLIB_LAS})
target_link_libraries(qtlasviewer qtlasviewer_library ${IRPLIB_LAS})





