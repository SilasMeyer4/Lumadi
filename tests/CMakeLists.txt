# Catch2 installieren (falls nicht schon im System vorhanden)
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

# Test-Binary bauen
add_executable(test_threadpool test_staticThreadPool.cpp test_workStealingQueue.cpp)
target_link_libraries(test_threadpool PRIVATE Lumadi_ThreadPool Catch2::Catch2WithMain)

# Als Test registrieren
add_test(NAME ThreadPoolTests COMMAND test_threadpool)