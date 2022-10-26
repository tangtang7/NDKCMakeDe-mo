# NDKCMakeDemo
学习Android Studio使用NDK的第一个程序（ CMake 方式，适用于AS3之后）

1) 创建项目：New Project -> 选择 native c++ -> 可在 c++ standard 中更改 c++ 标准，默认为 Toolchain Default （Cmake 所使用的标准）。
	• 在 IDE 左侧打开 Project 窗格，然后选择 Android 视图 ，cpp 文件夹中，可以找到项目中的所有原生源代码文件、头文件、CMake 或 ndk-build 的构建脚本，以及项目中的预构建库。
	• 新项目中，Android Studio 会创建一个示例 C++ 源代码文件 native-lib.cpp、CMakeLists.txt，并在 build.gradle 中进行设置。C++ 文件置于应用模块的 src/main/cpp/ 目录中。此示例代码提供了一个简单的 C++ 函数 stringFromJNI()，它会返回字符串“Hello from C++”。
	• 构建项目后，在 build/intermediates/cmake/debug/obj/ 路径下不同的 ABI 目录中会生成 .so 文件。
2) 在任意 java 文件中声明 native 方法，按下 shift + option + enter，Android Studio 会自动在 native-lib.cpp 中生成对应的函数，在函数中进行实现即可。
3) 创建新的 C/C++ 源代码文件添加到现有项目：
	1- 如果项目的主源代码集内还没有 cpp/ 目录，则需要创建：
		a. 从 IDE 的左侧打开 Project 窗格，然后从下拉菜单中选择 Project 视图。
		b. 依次转到 your-module > src，右键点击 main 目录，然后依次选择 New > Directory。
		c. 输入 cpp 作为目录名称，然后点击 OK。
	2- 右键点击 cpp/ 目录，然后依次选择 New > C/C++ Source File。
	3- 为源代码文件输入一个名称，例如 native-lib。
	4- 从 Type 下拉菜单中，为源代码文件选择文件扩展名， .cpp / .c。（可以向此下拉菜单添加其他文件类型（例如 .cxx 或 .hxx），点击 Edit File Types 图标，在弹出的 C/C++ 对话框中，从 Source Extension 和 Header Extension 下拉菜单中选择其他文件扩展名，然后点击 OK）
	5- 如果想要创建头文件，则勾选 Create an associated header 复选框。
4) 配置 Cmake ：
	1- CMake 构建脚本是一个纯文本文件，必须将其命名为 CMakeLists.txt，并在其中包含 CMake 构建 C/C++ 库时需要使用的命令。
	2- 如果项目中还没有 CMake 构建脚本
		a. 自行创建。在 cpp 文件夹下 New -> File，文件命名为 “CMakeLists.txt”。
		b. 并在其中包含适当的 CMake 命令。（eg. 指示 CMake 根据原生源代码创建原生库需要添加 cmake_minimum_required()  和 add_library()）。
	3-  CMake 命令官方文档，举例：构建脚本中包含哪些基本命令
		a. cmake_minimum_required()：设置构建本地库所需的 Cmake 的最小版本。
		b. add_library() ：向 CMake 构建脚本添加源代码文件或库（自定义原生库、以源代码形式存在的库、其他与构建库）。多个 add_library 可以生成多个库。
			® 创建并命名一个库，将其设置为 STATIC 或 SHARED，并提供其源代码的相对路径。可以定义多个库，Cmake 进行构建。Gradle 自动将共享库打包到 APK 中。
			® 代码举例：
				add_library( #设置库（编译出来的so包）的名称，不需要添加lib前缀。
					jnitest
					# 设置为共享链接库。
					# 指定要创建的库的类型: STATIC、SHARED 或 MODULE 可以。
					# 静态库是目标文件归档，链接其他目标时使用。
					# 共享库被动态链接并在运行时加载。
					# MODULE 库不链接到其他目标，但可以在运行时使用类似 dlopen 的功能动态加载。
					# 如果没有显式给出类型，则根据变量 BUILD_SHARED_LIBS 的当前值是否为 ON，决定类型为 STATIC 或 SHARED。
					# 对于 SHARED 和 MODULE 库，POSITION_INDEPENDENT_CODE 目标属性会自动设置为 ON。
					# 共享库可以用 FRAMEWORK 目标属性进行标记，以创建 macOS 框架。
					SHARED
					# 设置源文件的相对路径，可将多个源文件进行编译，以空格或换行区分多个文件。
					testh/jniTest2.c jniTest1.c jniTest3.c)
		c. include_directories()：指定头文件的路径。
			® Android Studio 会在同步项目后在 Project 视图中显示相关的头文件，为了让 CMake 能够在编译时找到头文件，需要向 CMake 添加命令指定头文件的路径（实测不加也未报错）。
			® 代码举例：
			# 指定本地头文件的路径。
			include_directories(src/main/cpp/include/)
			include_directories(src/main/cpp/testh/)
		d. add_executable() 指示 CMake 改为根据这些源代码文件来创建可执行文件。不过，根据原生源代码文件构建可执行文件是可选操作，构建原生库以将其打包到 APK 中即可满足大多数项目的要求。
		e. find_library() ：搜索指定的预构建 NDK 库，并将其路径存储为变量。可以使用此变量在构建脚本的其他部分引用 NDK 库。
			® Android 提供了一套的原生 API 和库，存在预构建的 NDK 库。因为 Cmake 默认在搜索路径中包含系统库，因此无需指定本地安装的 NDK 库的位置、构建 NDK 库或将其打包到 APK 中。通过在项目的 CMakeLists.txt 脚本文件中指定想要添加的 NDK 库名称，并将其与自己的库相关联，就可以使用其中任何 API。Cmake 会在完成构建之前验证库是否存在。
			® 代码举例：
				find_library(# 设置外部引用库路径变量的名称。
					log-lib
					# 指定想要CMake定位的NDK库（外部引用库）的名称。log库支持在c/c++中打印log，具体请见android/log.h
					log )
		f.  target_link_libraries() ：关联库，指定CMake应该链接到目标库的库。可以链接多个库，例如在此构建脚本中定义的库、预构建的第三方库或系统库。
			® 代码举例：
				target_link_libraries( # 指定被链接的库。
					ndkcmakedemo
					# 将目标库链接到 NDK 中包含的日志库，可以链接多个库，以空格或换行区分。
					${log-lib} )
		g. 添加以源代码形式存在的库：举例：将 android_native_app_glue.c（负责管理 NativeActivity 生命周期事件和触摸输入）构建至静态库，并将其与 appGlueTest 关联
			a) 需要自行构建这些代码并将其关联到自己的原生库。
			b) 代码示例：官方（native-activity）、NDKCmakeDemo - appGlueTest
			c) 使用 add_library() 将源代码编译到原生库中。如需提供本地 NDK 库的路径，可以使用 Android Studio 自动已定义的 ANDROID_NDK 路径变量。
				add_library(native_app_glue
					STATIC
					${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
			d) target_include_directories()：指定目标库包含的头文件路径。
				target_include_directories(appGlueTest
					PRIVATE
					${ANDROID_NDK}/sources/android/native_app_glue)
			e) 链接源代码库到目标库
				target_link_libraries( appGlueTest
					native_app_glue )
		h. 添加其他预构建库
			a) 由于库已构建，需要使用 IMPORTED 标志指示 CMake 只想要将此库导入到项目中：
				add_library( imported-lib
					SHARED
					IMPORTED )
			b) 使用 set_target_properties() 指定库的路径。
			某些库会针对特定的 CPU 架构或应用二进制接口 (ABI) 提供单独的软件包，并将其整理到单独的目录中。此方法既有助于库充分利用特定的 CPU 架构，又能让用户只使用所需的库版本。如果想向 CMake 构建脚本添加库的多个 ABI 版本，而不必为库的每个版本编写多个命令，可以使用 ANDROID_ABI 路径变量。此变量使用的是 NDK 支持的一组默认 ABI，或者使用手动配置 Gradle 的一组经过过滤的 ABI。代码：
				set_target_properties( # 指定目标库。
					imported-lib
					# 指定要定义的参数。
					PROPERTIESIMPORTED_LOCATION
					# 提供要导入的库的路径。
					imported-lib/src/${ANDROID_ABI}/libimported-lib.so)
			c) 如果想要打包不属于构建时依赖项的预构建库（例如在添加属于 imported-lib 依赖项的预构建库时），则无需执行关联库的操作。如需将预构建库关联到自己的原生库，代码：
				target_link_libraries(importTest
					imported-lib)
			d) 如需将预构建库打包到 APK 中，需要使用 sourceSets 块手动配置 Gradle 以包含 .so 文件的路径。构建 APK 后，可以使用 APK 分析器验证 Gradle 会将哪些库打包到您的 APK 中。
		i. 包含其他 CMake 项目
			a) 如果想要构建多个 CMake 项目并在 Android 项目中包含这些 CMake 项目的输出，可以使用一个 CMakeLists.txt 文件（即关联到 Gradle 的那个文件）作为顶级 CMake 构建脚本，并添加其他 CMake 项目作为此构建脚本的依赖项。
			b) 顶级 CMake 构建脚本使用 add_subdirectory() 命令将另一个 CMakeLists.txt 文件指定为构建依赖项，然后关联其输出，就像处理任何其他预构建库一样。
		j. 从命令行调用 CMake：
			a) 使用以下命令调用 CMake 以在 Android Studio 外生成 Ninja 项目：
			cmake
			-Hpath/to/cmakelists/folder
			-Bpath/to/generated/ninja/project/debug/ABI
			-DANDROID_ABI=ABI                                            // For example, arm64-v8a
			-DANDROID_PLATFORM=platform-version-string       // For example, android-16
			-DANDROID_NDK=android-sdk/ndk/ndk-version
			-DCMAKE_TOOLCHAIN_FILE=android-sdk/ndk/ndk-version/build/cmake/android.toolchain. cmake
			-G Ninja
			b) 执行此命令所生成的 Ninja 项目可创建 Android 可执行文件库（.so 文件）。使用 NDK 的 CMake 支持需要使用 CMAKE_TOOLCHAIN_FILE。对于 CMake 3.21 或更高版本，您可以改用 CMake 的内置 NDK 支持，但必须使用 CMake 的 Android 交叉编译文档中介绍的一组不同变量。
			c) 在 CMake 3.20 之前，使用内置 CMake 支持会导致截然不同的构建行为，而 CMake 3.20 统一了实现这两个接口的实现，相应 bug 直到 3.21 才得以解决。
			d) 如果 CMake 找不到 Ninja 可执行文件，请使用以下可选参数：
				-DCMAKE_MAKE_PROGRAM=path/to/ninja/ninja.exe。
			e) 针对报错：2 files found with path 'lib/arm64-v8a/libgmath.so' from inputs:
				Module 的 build.gradle 的 android{} 加上：
				    sourceSets {
					main {
					    jniLibs.srcDirs = ['libs']
					}
				    }
5) 将Gradle 关联到原生库
	1- 如需添加原生库项目作为 Gradle 构建依赖项，需要向 Gradle 提供 CMake 或 ndk-build 脚本文件的路径。构建应用时，Gradle 会运行 CMake 或 ndk-build，并将共享的库与应用打包到一起。Gradle 还会使用构建脚本来了解要将哪些文件添加到 Android Studio 项目中，以便从 Project 窗口访问这些文件。
	2- Android 项目中的每个模块只能关联到一个 CMake 或 ndk-build 脚本文件。如果想要构建并打包来自多个 CMake 项目的输出，就需要使用一个 CMakeLists.txt 文件作为顶级 CMake 构建脚本（然后将 Gradle 关联到该脚本），并添加其他 CMake 项目作为该构建脚本的依赖项。同样，如果使用 ndk-build，可以在顶级 Android.mk 脚本文件中包含其他 Makefile。
	3- 将 Gradle 关联到原生项目后，Android Studio 会更新 Project 窗格，以在 cpp 组中显示源代码文件和原生库，并在 External Build Files 组中显示外部构建脚本。更改 Gradle 配置后，需要点击工具栏中的 Sync Project 图标，让所做的更改生效。此外，如果在将 CMake 或 ndk-build 脚本文件关联到 Gradle 之后要对文件进行更改，需要从菜单栏中依次选择 Build > Refresh Linked C++ Projects，将 Android Studio 与更改进行同步。
	4- 使用 Android Studio 界面将 Gradle 关联到外部 CMake 或 ndk-build 项目：
		a. 从 IDE 左侧打开 Project 窗格，然后选择 Android 视图。
		b. 右键点击想要关联到原生库的模块（例如 app 模块），然后从菜单中选择 Link C++ Project with Gradle。
		c. 从下拉菜单中，选择 CMake 或 ndk-build。
		d. 如果选择 Cmake，Project Path 填写外部 CMake 项目指定 CMakeLists.txt 脚本文件。
		e. 如果选择 ndk-build，Project Path 天蝎外部 ndk-build 项目指定 Android.mk 脚本文件。如果 Application.mk 文件与 Android.mk 文件位于同一目录下，Android Studio 也会包含此文件。
		f. 实测：右键选择 Add C++ to Module
	5- 手动配置 Gradle
		a. 手动配置 Gradle 以关联到原生库，需要将 externalNativeBuild 块添加到模块级 build.gradle 文件中，并使用 cmake 或 ndkBuild 块对其进行配置：
			a) 代码举例：
				android {
					...
					defaultConfig {...}
					buildTypes {...}
					// Encapsulates your external native build configurations.
					externalNativeBuild {
						// Encapsulates your CMake build configurations.
						cmake {
							// Provides a relative path to your CMake build script.
							path "CMakeLists.txt"
						}
					}
				}
			b) 如果要将 Gradle 关联到现有的 ndk-build 项目，请使用 ndkBuild 块（而不是 cmake 块），并提供指向 Android.mk文件的相对路径。如果 Application.mk 文件与您的 Android.mk 文件位于同一目录下，Gradle 也会包含此文件。
	6- 指定可选配置
		a. 可以在模块级 build.gradle 文件的 defaultConfig 块中配置另一个 externalNativeBuild 块，以为 CMake 或 ndk-build 指定可选参数和标记。与 defaultConfig 块中的其他属性类似，也可以在构建配置中为每个产品变种替换这些属性。
		b. 例如，如果 CMake 或 ndk-build 项目定义了多个原生库和可执行文件，可以使用 targets 属性为指定产品变种构建和打包其中的部分工件。可以配置的部分属性代码举例：
			android {
				...
				defaultConfig {
					...
					// This block is different from the one you use to link Gradle to your Cmake or ndk-build script.
					// 此模块与用来链接 Gradle 的 Cmake 或 ndk-build 脚本的模块不同
					externalNativeBuild {
						//For ndk-build,instead use the ndkBuild block.
						//对于 ndk-build，使用 ndkBuild 替换
						cmake{
							//Passes optional arguments to CMake. 将可选参数传递给 CMake。
							arguments "-DANDROID_ARM_NEON=TRUE","-DANDROID_TOOLCHAIN=clang"
							//Sets a flag to enable format macro constants for the C compiler.
							//设置一个标志以启用 C 编译器的格式宏常量。
							cFlags "-D__STDC_FORMAT_MACROS"

							//Sets optional flags for the C++ compiler.
							//为 C++ 编译器设置可选标志。
							cppFlags "-fexceptions","-frtti"
						}
					}
				}
				buildTypes {...}
				productFlavors {
					...
					demo {
						...
						externalNativeBuild {
						cmake {
							//指定要为此产品风格构建和打包哪些原生库或可执行程序。
							//下面告诉Gradle只从链接的CMake项目中构建“native-lib-demo”和“my-execuitable-demo”输出。
							//如果没有配置这个属性，Gradle会构建你在CMake(或ndk-build)项目中定义的所有可执行文件和共享对象库。
							//但是，默认情况下，Gradle只打包应用程序中的共享库。
							targets"native-lib-demo",
							//需要在CMakeLists.txt中使用add_executable()命令指定这个可执行文件及其源文件。
							//然而，从本机源代码构建可执行程序是可选的，且构建本机库打包到应用程序中可以满足大多数项目需求。
							"my-executible-demo"
						}
					}
				}
				paid{
					...
					externalNativeBuild{
						cmake{
							...
							targets "native-lib-paid","my-executible-paid"
						}
					}
				}
			}
		c. 如需详细了解如何配置产品变种和 build 变体，请参阅配置 build 变体。如需了解可以使用 arguments 属性为 CMake 配置的变量列表，请参阅使用 CMake 变量。
	7- 添加预构建的原生库
		a. 如果希望 Gradle 打包未在任何外部原生 build 中使用的预构建原生库，请将其添加到模块的 src/main/jniLibs/ABI 目录中。
		b. 4.0 之前的 Android Gradle 插件版本要求 CMake IMPORTED 目标必须包含在 jniLibs 目录中才能将这些目标纳入应用。如果要从该插件的较低版本进行迁移，可能会遇到如下错误：
			* What went wrong:
				Execution failed for task ':app:mergeDebugNativeLibs'.
					> A failure occurred while executing com.android.build.gradle.internal.tasks.Workers$ActionFacade
					> More than one file was found with OS independent path 'lib/x86/libprebuilt.so'
			如果使用的是 Android Gradle 插件 4.0，请将 IMPORTED CMake 目标使用的所有库从 jniLibs 目录移出，以避免出现此错误。
	8- 指定 ABI
		a. 默认情况下，Gradle 会针对 NDK 支持的应用二进制接口 (ABI) 将您的原生库构建为单独的 .so 文件，并将这些文件全部打包到应用中。如果希望 Gradle 仅构建和打包原生库的特定 ABI 配置，可以在模块级 build.gradle 文件中使用 ndk.abiFilters 标志指定这些配置，如下所示：
			android {
				...
				defaultConfig {
					...
					externalNativeBuild {
						cmake {...}
						// or ndkBuild {...}
					}
					// 类似于defaultConfig模块中的其他属性，可以在构建配置中为每种产品类型配置ndk模块。    
					ndk {
						// 类似于defaultConfig模块中的其他属性，可以在构建配置中为每种产品类型配置ndk模块。
						abiFilters  'x86', 'x86_64', 'armeabi', 'armeabi-v7a','arm64-v8a'
					}
				}
				buildTypes {...}
				externalNativeBuild {...}
			}
		b. 在大多数情况下，只需要在 ndk 块中指定 abiFilters（如上所示），因为它会指示 Gradle 构建和打包原生库的这些版本。但是，如果想控制 Gradle 构建的配置，且与打包到应用中的配置相互独立，请在 defaultConfig.externalNativeBuild.cmake （或 defaultConfig.externalNativeBuild.ndkBuild ）中配置另一个 abiFilters 标志。Gradle 会构建这些 ABI 配置，但只会打包在 defaultConfig.ndk 块中指定的配置。
		c. 建议使用 Android App Bundle 格式发布应用以进一步减小应用的大小，因为只有与用户设备的 ABI 相匹配的原生库才会通过下载内容分发。
		d. 对于使用 APK 发布的旧版应用（创建于 2021 年 8 月之前），请考虑基于 ABI 配置多个 APK，而不是创建一个包含原生库所有版本的大型 APK。Gradle 会为您想要支持的每个 ABI 创建单独的 APK，并且仅打包每个 ABI 需要的文件。如果每个 ABI 配置了多个 APK，但没有像上面的代码示例中所示的那样指定 abiFilters 标志，Gradle 会为原生库构建所有受支持的 ABI 版本，但是仅打包在多 APK 配置中指定的版本。为避免构建不想要的原生库版本，请为 abiFilters 标记和“一个 ABI 多个 APK”配置提供相同的 ABI 列表。
6) 在 Java 或 Kotlin 代码中用 System.loadLibrary() 方法加载链接库文件，使用静态域包裹将so包加载进来。
	1- Java 中
		static {
			System.loadLibrary("jnitest");
		}
	2- Kotlin 中
		companion object {
			init {
				System.loadLibrary("native-lib");
			    }
		}
7) 如果重命名或移除了 CMake 构建脚本中的库，需要先清理项目（ Build -> Clean Project），然后再 Gradle 应用相关更改或从 APK 中移除旧版库。
8) 生成的.so文件的路径：app\build\intermediates\cmake\debug\obj。
