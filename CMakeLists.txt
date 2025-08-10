cmake_minimum_required(VERSION 3.10)
project(Lumadi VERSION 0.1.0 LANGUAGES CXX)

# Add each library/component as a subdirectory
add_subdirectory(ThreadPool)


# Optionally, add other components in the future:
# add_subdirectory(Logger)
# add_subdirectory(OtherComponent)

# Super-Paket-Config für das Gesamtpaket Lumadi
include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/LumadiConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/LumadiConfig.cmake"
	INSTALL_DESTINATION lib/cmake/Lumadi
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/LumadiConfig.cmake"
	DESTINATION lib/cmake/Lumadi
)

set(CPACK_PACKAGE_NAME "Lumadi")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")

include(CPack)

enable_testing()
