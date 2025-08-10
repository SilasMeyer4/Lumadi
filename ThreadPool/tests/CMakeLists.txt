# installing cache2
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

# test binary
add_executable(test_threadpool test_staticThreadPool.cpp test_workStealingQueue.cpp)
target_link_libraries(test_threadpool PRIVATE ThreadPool Catch2::Catch2WithMain)
target_include_directories(test_threadpool PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# register test
add_test(NAME ThreadPoolTests COMMAND test_threadpool)