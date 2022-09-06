# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

if (IDF_VERSION_MAJOR LESS 4)
    message(FATAL_ERROR "ESP32IO requires IDF v4.2 (or newer)")
endif()

if (IDF_VERSION_MAJOR EQUAL 4 AND IDF_VERSION_MINOR LESS 2)
    message(FATAL_ERROR "ESP32IO requires IDF v4.2 (or newer)")
endif()

set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../OpenMRNIDF")

set(SUPPORTED_TARGETS esp32)
project(Esp32PWMHalfSiding)

###############################################################################
# Switch from GNU++11 to GNU++14
#
# NOTE: C++14 would be preferred but IDF has dependencies on GNU extensions.
###############################################################################

string(REPLACE "-std=gnu++11" "-std=gnu++14" CXX_OPTIONS "${CXX_COMPILE_OPTIONS}")
idf_build_set_property(CXX_COMPILE_OPTIONS "${CXX_OPTIONS}" REPLACE)

###############################################################################
# Enable usage of std::stoi/stol/etc
###############################################################################

idf_build_set_property(COMPILE_DEFINITIONS "-D_GLIBCXX_USE_C99" APPEND)

###############################################################################
# Uncomment the below to enable NDEBUG build mode
# NOTE: This disables some error checking in ESP-IDF and may lead to unexpected
# behavior in a few areas. More testing is necessary before this can be enabled
# by default.
###############################################################################

# idf_build_set_property(COMPILE_DEFINITIONS "-DNDEBUG" APPEND)
# idf_build_set_property(COMPILE_DEFINITIONS "-Wno-unused-variable" APPEND)
# idf_build_set_property(COMPILE_DEFINITIONS "-Wno-unused-but-set-variable" APPEND)

###############################################################################
# Verify submodules were checked out correctly
###############################################################################

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../OpenMRNIDF/CMakeLists.txt)
    message(FATAL_ERROR  "${CMAKE_CURRENT_SOURCE_DIR}/../OpenMRNIDF does not appear to have been checked out, please run 'git submodule  update --init' from ${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()

###############################################################################
# Configuration validations
###############################################################################

if (NOT CONFIG_FREERTOS_HZ EQUAL 1000)
    message(FATAL_ERROR "FreeRTOS tick rate (hz) is required to be 1000.")
endif()

if (NOT CONFIG_PARTITION_TABLE_FILENAME STREQUAL "esp32pwmhalfsiding-partitions.csv")
    message(FATAL_ERROR "The custom partition table option is not enabled in menuconfig and is required for compilation.")
endif()

if (NOT CONFIG_PARTITION_TABLE_CUSTOM_FILENAME STREQUAL "esp32pwmhalfsiding-partitions.csv")
    message(FATAL_ERROR "The custom partition table option is not enabled in menuconfig and is required for compilation.")
endif()

if (NOT CONFIG_PARTITION_TABLE_CUSTOM)
    message(FATAL_ERROR "The custom partition table option is not enabled in menuconfig and is required for compilation.")
endif()

