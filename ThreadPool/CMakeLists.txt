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

enable_testing()
add_subdirectory(tests)