cmake_minimum_required(VERSION 3.14)

project(789 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)
find_package(Python COMPONENTS Development)

# 檢查是否找到Python
if(Python_FOUND)
    message(STATUS "Python found: ${Python_VERSION}")
    message(STATUS "Python include directories: ${Python_INCLUDE_DIRS}")

    # 將Python的include目錄添加到項目中
    include_directories(${Python_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Python not found. Please install Python development files.")
endif()


include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(789
  main.cpp
  imageprocess.h imageprocess.cpp
  structImage.h
)
# 链接Python庫
target_link_libraries(789 ${Python_LIBRARIES})
target_link_libraries(789 Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(789 Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(789 ${OpenCV_LIBS} )

include(GNUInstallDirs)
install(TARGETS 789
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
