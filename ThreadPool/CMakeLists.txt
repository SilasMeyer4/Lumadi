project(Lumadi_ThreadPool VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB THREADPOOL_SOURCES src/*.cpp)
add_library(Lumadi_ThreadPool ${THREADPOOL_SOURCES})

target_include_directories(Lumadi_ThreadPool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


install(TARGETS Lumadi_ThreadPool
    EXPORT Lumadi_ThreadPoolTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/Lumadi/ DESTINATION include/Lumadi)

# Export targets and create CMake package config files
install(EXPORT Lumadi_ThreadPoolTargets
    FILE Lumadi_ThreadPoolTargets.cmake
    NAMESPACE Lumadi::
    DESTINATION lib/cmake/Lumadi_ThreadPool
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/Lumadi_ThreadPoolConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Lumadi_ThreadPoolConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Lumadi_ThreadPoolConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Lumadi_ThreadPool
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/Lumadi_ThreadPoolConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Lumadi_ThreadPoolConfigVersion.cmake"
    DESTINATION lib/cmake/Lumadi_ThreadPool
)

enable_testing()
add_subdirectory(tests)