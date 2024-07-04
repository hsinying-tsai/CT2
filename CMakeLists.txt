cmake_minimum_required(VERSION 3.5)

project(0301_1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(OpenCV_DIR C:\\Apps\\OpenCV\\opencv\\build)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widget REQUIRED)
find_package(pylon)
find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
        tcp_client.h
        tcp_client.cpp
        my_qlabel.cpp
        my_qlabel.h
        logger.cpp
        logger.h
        ini.h
        ini.cpp
        myFuncts.cpp
        myFuncts.h
        defineCoordinate.h
        defineCoordinate.cpp
        guicamera.h
        guicamera.cpp
        maindialog.h
        maindialog.cpp
        maindialog.ui
        funcpar.h
        funcpar.cpp
        funcpar.ui
        structImage.h
        imageprocess.h
        imageprocess.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(0301_1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        my_qlabel.h my_qlabel.cpp
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 0301_1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(0301_1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(0301_1
            ${PROJECT_SOURCES}
        )
    endif()
endif()
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/opt/pylon/include")

include_directories(${MYSQL_INCLUDE_DIR})

message(opencv_INCLUDE_DIRS="${OpenCV_INCLUDE_DIRS}")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(0301_1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(0301_1 PRIVATE ${OpenCV_LIBS})
target_link_libraries(0301_1 PRIVATE ${MYSQL_LIBRARIES})
target_link_libraries(0301_1 PRIVATE
    -L"/opt/pylon/lib"
    -lpylonbase
    -lpylonutility
    -lGenApi_gcc_v3_1_Basler_pylon
    -lGCBase_gcc_v3_1_Basler_pylon)
target_link_libraries(0301_1 PRIVATE pylon::pylon )
target_link_libraries(0301_1 PRIVATE Qt5::Widgets Qt5::Sql)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.0301_1)
endif()
set_target_properties(0301_1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
#if(iniparser)
#    include_directories(/home/agx/Desktop/0321_qt/iniparser/src)
include(GNUInstallDirs)
install(TARGETS 0301_1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(0301_1)
endif()
