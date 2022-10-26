# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
# 设置构建本地库所需的 CMake 的最小版本。
cmake_minimum_required(VERSION 3.18.1)

#set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /Users/whh/AndroidStudioProjects/NDKCMakeDemo/app/src/main/gmath/outputs )

add_library(gmath
        SHARED
        gmath.c)

set_target_properties(gmath
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${ANDROID_ABI})

## copy out the lib binary... need to leave the static lib around to pass gradle check
#set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../distribution)
#set_target_properties(gmath
#        PROPERTIES
#        ARCHIVE_OUTPUT_DIRECTORY
#        "${distribution_DIR}/gmath/lib/${ANDROID_ABI}")
#
## copy out lib header file...
#add_custom_command(TARGET gmath POST_BUILD
#        COMMAND "${CMAKE_COMMAND}" -E
#        copy "${CMAKE_CURRENT_SOURCE_DIR}/src/gmath.h"
#        "${distribution_DIR}/gmath/include/gmath.h"
#        #                   **** the following 2 lines are for potential future debug purpose ****
#        #                   COMMAND "${CMAKE_COMMAND}" -E
#        #                   remove_directory "${CMAKE_CURRENT_BINARY_DIR}"
#        COMMENT "Copying gmath to output directory")