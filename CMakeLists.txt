cmake_minimum_required(VERSION 3.10)
project(Lumadi VERSION 0.1.0 LANGUAGES CXX)

# Add each library/component as a subdirectory
add_subdirectory(ThreadPool)

# Optionally, add other components in the future:
# add_subdirectory(Logger)
# add_subdirectory(OtherComponent)
