# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
# 设置构建本地库所需的 CMake 的最小版本。
cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("ndkcmakedemo")

# Creates and names a library, sets it as either STATIC or SHARED,
# and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
# 创建并命名一个库，将其设置为 STATIC 或 SHARED，并提供其源代码的相对路径。
# 可以定义多个库，CMake 进行构建。
# Gradle 自动将共享库打包到 APK 中。
add_library( # Sets the name of the library. 设置库（编译出来的so包）的名称，不需要添加lib前缀。
        ndkcmakedemo

        # Sets the library as a shared library. 设置为共享链接库.
        # 指定要创建的库的类型: STATIC、SHARED或MODULE可以。
        # 静态库是目标文件归档，链接其他目标时使用。
        # 共享库被动态链接并在运行时加载。
        # MODULE库不链接到其他目标，但可以在运行时使用类似 dlopen 的功能动态加载。
        # 如果没有显式给出类型，则根据变量 BUILD_SHARED_LIBS 的当前值是否为 ON，决定类型为 STATIC 或 SHARED。
        # 对于 SHARED 和 MODULE 库，POSITION_INDEPENDENT_CODE 目标属性会自动设置为 ON。
        # 共享库可以用 FRAMEWORK 目标属性进行标记，以创建 macOS 框架。
        SHARED

        # Provides a relative path to your source file(s).
        # 设置源文件的相对路径，可将多个源文件进行编译，以空格或换行区分多个文件。
        native-lib.cpp)

add_library( jnitest
        MODULE
        hFileTest/jniTest2.c jniTest1.c jniTest3.c)

# Specifies a path to native header files. 指定本地头文件的路径。（实测不加也能正常使用）
include_directories(src/main/cpp/include/)
include_directories(src/main/cpp/testh/)
include_directories(src/main/cpp/)

# Searches for a specified prebuilt library and stores the path as a variable.
# Because CMake includes system libraries in the search path by default,
# you only need to specify the name of the public NDK library you want to add.
# CMake verifies that the library exists before completing its build.
# 搜索指定的预构建库，并将路径存储为变量。
# 因为 CMake 默认在搜索路径中包含系统库，只需要指定想要添加的公共 NDK 库的名称。
# CMake在完成构建之前验证库是否存在。
find_library( # Sets the name of the path variable.
        # 设置外部引用库路径变量的名称。
        log-lib

        # Specifies the name of the NDK library that you want CMake to locate.
        # 指定想要 CMake 定位的 NDK 库（外部引用库）的名称。log 库支持在 c/c++ 中打印 log，具体请见  android/log.h
        log)

# Specifies libraries CMake should link to your target library.
# You can link multiple libraries, such as libraries you define in this build script,
# prebuilt third-party libraries, or system libraries.
# 指定CMake应该链接到目标库的库。
# 可以链接多个库，例如在此构建脚本中定义的库、预构建的第三方库或系统库。
target_link_libraries( # Specifies the target library. 指定被链接的库。
        ndkcmakedemo

        # Links the target library to the log library included in the NDK.
        # 将目标库链接到 NDK 中包含的日志库，可以链接多个库，以空格或换行区分。即链接 log-lib 到 ndkcmakedemo。
        ${log-lib} )





# 添加以源代码形式存在的库
# 举例：将 android_native_app_glue.c（负责管理 NativeActivity 生命周期事件和触摸输入）构建至静态库
# 官方示例代码：https://github.com/android/ndk-samples/tree/main/native-activity
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
# 使用 CMake 构建脚本中的 add_library() 将以源代码形式存在的库编译到原生库中。
# 可以使用 Android Studio 已定义的 ANDROID_NDK 路径变量，提供本地 NDK 库的路径。
add_library( native_app_glue
        STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c )
# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library( appGlueTest
        SHARED
        ../cpp/appGlueTest/appgluetest.cpp)

# 指定目标库包含相应头文件的路径（必须加），也可用 include_directories()
target_include_directories( appGlueTest
        PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

# add lib dependencies。添加依赖。
target_link_libraries(appGlueTest
        android
        native_app_glue
        EGL
        GLESv1_CM
        log)





# 添加其他预构建库
# 由于库已构建，需要使用 IMPORTED 标志指示 CMake 只想要将此库导入到项目中：
add_library( imported-lib
        SHARED
        IMPORTED )

# 指定库的路径
set_target_properties( # Specifies the target library. 指定目标库。
        imported-lib
        # Specifies the parameter you want to define. 指定要定义的参数。
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import. 提供要导入的库的路径。
        /Users/whh/AndroidStudioProjects/NDKCMakeDemo/app/src/main/importTest/libimportTest.so )
# 官方示例：imported-lib/src/${ANDROID_ABI}/libimported-lib.so == /Users/whh/AndroidStudioProjects/NDKCMakeDemo/app/build/intermediates/cxx/Debug/5c6i1e3x/obj/arm64-v8a/libimportTest.so

# 包含相应头文件的路径
##target_include_directories( ndkcmakedemo
##        PRIVATE
##        imported-lib/include/)
## 或
include_directories( imported-lib/include/ )

# 关联库
target_link_libraries( ndkcmakedemo
        imported-lib)





# 包含其他 CMake 项目
add_library( cmaketest
        SHARED
        jniTest3.c)
# Sets lib_src_DIR to the path of the target CMake project. 将 lib_src_DIR 设置为目标 CMake 项目的路径。
set( lib_src_DIR /Users/whh/AndroidStudioProjects/NDKCMakeDemo/app/src/main/gmath )
# Sets lib_build_DIR to the path of the desired output directory. 将 lib_build_DIR 设置为输出目录的路径。
set( lib_build_DIR /Users/whh/AndroidStudioProjects/NDKCMakeDemo/app/src/main/gmath/outputs )
file(MAKE_DIRECTORY ${lib_build_DIR})

# Adds the CMakeLists.txt file located in the specified directory as a build dependency.
# 将位于指定目录中的 CMakeLists.txt 文件添加为构建依赖项。
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
        # 指定CMakeLists.txt文件的目录。
        ${lib_src_DIR}

        # Specifies the directory for the build outputs. 指定构建输出的目录。
        ${lib_build_DIR} )

# Adds the output of the additional CMake build as a prebuilt static library and names it lib_gmath.
# 将额外的 CMake 构建的输出添加为预构建库，并将其命名为 lib_gmath。
add_library( lib_gmath SHARED IMPORTED )
set_target_properties( lib_gmath
        PROPERTIES IMPORTED_LOCATION
        ${lib_build_DIR}/${ANDROID_ABI}/libgmath.so )
include_directories( ${lib_src_DIR} )

# Links the top-level CMake build output against lib_gmath.
# 将顶级 CMake 构建输出链接到 lib_gmath。
#target_link_libraries( cmaketest ... lib_gmath )
