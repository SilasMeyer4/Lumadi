cmake_minimum_required(VERSION 3.10)
project(Lumadi_ThreadPool VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library erstellen
file(GLOB THREADPOOL_SOURCES src/*.cpp)
add_library(Lumadi_ThreadPool ${THREADPOOL_SOURCES})

# Öffentliche Header bekannt machen
target_include_directories(Lumadi_ThreadPool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Installation (damit CPack später Pakete daraus bauen kann)
install(TARGETS Lumadi_ThreadPool
    EXPORT Lumadi_ThreadPoolTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/LumadiThreadPool/ DESTINATION include/LumadiThreadPool)

# CPack-Infos
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Tests aktivieren
enable_testing()
add_subdirectory(tests)
