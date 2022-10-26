//
// Created by whh on 2022/10/20.
//
#include <jni.h>

#ifndef NDKCMAKEDEMO_JNITEST2_H
#define NDKCMAKEDEMO_JNITEST2_H

JNIEXPORT jfloat JNICALL
Java_com_example_ndkcmakedemo_MainActivity_floatFromJNI(JNIEnv* env, jobject thiz, jfloat num1, jfloat num2);

JNIEXPORT jboolean JNICALL
Java_com_example_ndkcmakedemo_MainActivity_booleanFromJNI(JNIEnv *env, jobject thiz);
#endif //NDKCMAKEDEMO_JNITEST2_H
