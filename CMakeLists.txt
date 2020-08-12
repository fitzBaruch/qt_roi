cmake_minimum_required(VERSION 3.5)

project(guozi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()
set(OpenCV_DIR "/usr/local/opencv34/share/OpenCV")


find_package(Qt5 COMPONENTS Widgets  REQUIRED)
find_package(Qt5Network)

find_package(OpenCV REQUIRED)
set(OpenCV_LIB_DIR ${OenCV_INSTALL_PATH}/lib)

include_directories(${OpenCV_INCLUDE_DIRS})

if(ANDROID)
  add_library(guozi SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
  )
else()
  add_executable(guozi
    main.cpp
    mainwindow.cpp
    mainwindow.h
    LabelPress.cpp
    LabelPress.h

  )
endif()
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

#target_link_libraries(guozi ${OpenCV_LIBS})
target_link_libraries(guozi PRIVATE Qt5::Widgets Qt5::Network  ${OpenCV_LIBS} )
