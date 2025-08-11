# Lumadi

A modular C++20 library providing different tools and functionalities

## Features
- Static thread pool with work stealing

## Quick Start

### 1. FetchContent (Recommended)
```cmake
include(FetchContent)
FetchContent_Declare(
  Lumadi
  GIT_REPOSITORY https://github.com/SilasMeyer4/Lumadi.git
  GIT_TAG v0.1.0 # or tag of the latest release
)
FetchContent_MakeAvailable(Lumadi)

# For including a specifing part of the library. Here as an example the ThreadPool
target_link_libraries(your_target PRIVATE Lumadi::ThreadPool)

# For including the whole library
target_link_libraries(your_target PRIVATE Lumadi::Lumadi)
```

### 2. Cloning Repo and building yourself (doesn't work as smoothly as I want atleast with MSVC)
```bash

git clone https://github.com/SilasMeyer4/Lumadi.git
cd Lumadi

# For release
cmake -S . -B build-release
cmake --build build-release --config Release
cmake --install build-release --config Release --prefix ../Lumadi-install-release

# For Debug
cmake -S . -B build-debug
cmake --build build-debug --config Debug
cmake --install build-debug --config Debug --prefix ../Lumadi-install-debug
```

```cmake
find_package(Lumadi REQUIRED PATHS "${CMAKE_SOURCE_DIR}/lib/Lumadi-install-<here debug or release>/lib/cmake/lumadi")

target_include_directories(your_target PRIVATE $<TARGET_PROPERTY:Lumadi::ThreadPool,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(your_target PRIVATE Lumadi::ThreadPool)
```

### 3. Usage Example
```cpp
#include <lumadi/threadPool.h>
#include <future>

int AddInt(int a, int b) {
  return a + b;
}

int main() {
    Lumadi::StaticThreadPool<Lumadi::WorkStealingQueue> pool(4); // 4 is the amount of threads in this example

    // Using Lambda
    auto future1 = pool.AddTask([] { return 42; }); // returns std::future
    int result1 = future1.Get();

    // Using Function
    auto future2 = pool.AddTask(CompareInt, 4, 6);
    bool result2 = future2.Get();

    std::cout << "Lambda returned: " << result1 << " | Function returned: " << result2 << std::endl;
}

```


