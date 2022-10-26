//
// Created by whh on 2022/10/20.
//

#include "jnitest1.h"

JNIEXPORT jint JNICALL
Java_com_example_ndkcmakedemo_MainActivity_intFromJNI(JNIEnv *env, jobject thiz) {
    // TODO: implement intFromJNI()
    return   (*env)->GetVersion(env);
}