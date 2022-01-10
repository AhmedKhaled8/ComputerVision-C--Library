cmake_minimum_required(VERSION 3.0.0)
project(CVProject VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
FIND_PACKAGE (PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM gtkmm-3.0)

include_directories(include)
include_directories(imgs)
include_directories(${OpenCV_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})

set(SOURCES main.cpp
    src/GUI/GUI_init.cpp
    src/GUI/GUI_signals.cpp
    src/edges/canny.cpp
    src/edges/sobel.cpp
    src/edges/prewitt.cpp
    src/edges/roberts.cpp
    src/filters/average_filter.cpp
    src/filters/gaussian_filter.cpp
    src/filters/median_filter.cpp
    src/noise/gauss_noise.cpp
    src/noise/salt_pepper.cpp
    src/noise/uniform_noise.cpp
    src/FFT_Filter.cpp
    src/kernels_filter.cpp
    src/greyscale.cpp
    src/threshold.cpp
    src/normalization.cpp
    src/histogram.cpp
    src/equalization.cpp)

add_executable(CVProject ${SOURCES})

link_directories(${GTKMM_LIBRARY_DIRS})

target_link_libraries(CVProject ${OpenCV_LIBS} ${GTKMM_LIBRARIES})
target_link_libraries(CVProject fftw3)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
