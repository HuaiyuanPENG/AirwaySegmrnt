cmake_minimum_required(VERSION 2.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW) # CMake 3.10
endif()

project(AirwaySegment)

FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
    INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
    MESSAGE(FATAL_ERROR "Cannot build without ITK. Please set ITK_DIR")
ENDIF(ITK_FOUND)

find_package(VTK COMPONENTS
  vtkCommonCore
  vtkFiltersSources
  vtkGUISupportQt
  vtkIOImage
  vtkInteractionImage
)
INCLUDE(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

set( Srcs main.cpp mainwindow.cpp contrastadujestdialog.cpp)
set( Hdrs mainwindow.h itkImagetype.h contrastadujestdialog.h)
set( MOC_Hdrs mainwindow.h contrastadujestdialog.h)
set(UIs mainwindow.ui contrastadujestdialog.ui)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set( CMAKE_AUTOMOC ON )

if(VTK_QT_VERSION VERSION_GREATER "4")
    FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED QUIET)
    qt5_wrap_ui(UI_Srcs ${UIs})

    add_executable(${PROJECT_NAME} ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs})
    qt5_use_modules(${PROJECT_NAME} Core Gui Widgets)
    target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} ${ITK_LIBRARIES})
else()
    find_package(Qt4 REQUIRED)
    include (${QT_USE_FILE})

    set(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
    set(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
    set(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
    qt4_wrap_ui(UI_Srcs ${UIs})

    add_executable(${PROJECT_NAME} ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs})
    target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${VTK_LIBRARIES} ${ITK_LIBRARIES})
endif()

