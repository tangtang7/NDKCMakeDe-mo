#include <jni.h>
#include <string>
#include <android/log.h>

#define STRING_LOG "stringFromJNI"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndkcmakedemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    // 输出不同优先级的日志
    __android_log_print(ANDROID_LOG_VERBOSE, STRING_LOG, "ANDROID_LOG_VERBOSE: %s", hello.c_str());
    __android_log_print(ANDROID_LOG_DEBUG, STRING_LOG, "ANDROID_LOG_DEBUG: %s", hello.c_str());
    __android_log_print(ANDROID_LOG_INFO, STRING_LOG, "ANDROID_LOG_INFO: %s", hello.c_str());

    __android_log_print(ANDROID_LOG_WARN, STRING_LOG, "ANDROID_LOG_WARN: %s", hello.c_str());
    __android_log_print(ANDROID_LOG_ERROR, STRING_LOG, "ANDROID_LOG_ERROR: %s", hello.c_str());

    __android_log_print(ANDROID_LOG_SILENT, STRING_LOG, "ANDROID_LOG_SILENT: %s", hello.c_str());
    __android_log_print(ANDROID_LOG_FATAL, STRING_LOG, "ANDROID_LOG_FATAL: %s", hello.c_str());

    __android_log_print(ANDROID_LOG_DEFAULT, STRING_LOG, "ANDROID_LOG_DEFAULT: %s", hello.c_str());
    __android_log_print(ANDROID_LOG_UNKNOWN, STRING_LOG, "ANDROID_LOG_UNKNOWN: %s", hello.c_str());

    return env->NewStringUTF(hello.c_str());
}
