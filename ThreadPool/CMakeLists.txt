project(ThreadPool VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



file(GLOB THREADPOOL_SOURCES src/*.cpp)
add_library(ThreadPool ${THREADPOOL_SOURCES})
# ALIAS-Target für konsistente Nutzung
add_library(Lumadi::ThreadPool ALIAS ThreadPool)

target_include_directories(ThreadPool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)



install(TARGETS ThreadPool
    EXPORT ThreadPoolTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# only including threadPool.h
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lumadi/threadPool.h
    DESTINATION include/lumadi)


# Export targets and create CMake package config files
install(EXPORT ThreadPoolTargets
    FILE ThreadPoolTargets.cmake
    NAMESPACE Lumadi::
    DESTINATION lib/cmake/ThreadPool
)


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ThreadPoolConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ThreadPoolConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ThreadPoolConfig.cmake"
    INSTALL_DESTINATION lib/cmake/ThreadPool
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ThreadPoolConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ThreadPoolConfigVersion.cmake"
    DESTINATION lib/cmake/ThreadPool
)


# Option für Tests (Default: OFF)
option(BUILD_THREADPOOL_TESTS "Build ThreadPool tests" OFF)
if(BUILD_THREADPOOL_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()