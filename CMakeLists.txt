cmake_minimum_required(VERSION 3.10)
project(Lumadi VERSION 0.1.0 LANGUAGES CXX)

# Add each library/component as a subdirectory
add_subdirectory(ThreadPool)



# Optionally, add other components in the future:
# add_subdirectory(Logger)
# add_subdirectory(OtherComponent)

# Super package ALIAS target for build context (FetchContent, add_subdirectory)
if(NOT TARGET Lumadi::Lumadi)
  add_library(Lumadi_Lumadi INTERFACE)
  add_library(Lumadi::Lumadi ALIAS Lumadi_Lumadi)
  target_link_libraries(Lumadi_Lumadi INTERFACE Lumadi::ThreadPool)
endif()

# Super package config for the complete Lumadi package
include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/LumadiConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/LumadiConfig.cmake"
	INSTALL_DESTINATION lib/cmake/lumadi
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/LumadiConfig.cmake"
	DESTINATION lib/cmake/lumadi
)

set(CPACK_PACKAGE_NAME "Lumadi")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")

include(CPack)

enable_testing()
