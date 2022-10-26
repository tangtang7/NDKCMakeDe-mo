//
// Created by whh on 2022/10/19.
//

#include "jniTest2.h"

JNIEXPORT jfloat JNICALL
Java_com_example_ndkcmakedemo_MainActivity_floatFromJNI(JNIEnv* env, jobject thiz, jfloat num1, jfloat num2) {
    // TODO: implement floatFromJNI()
    return num1 + num2;
}