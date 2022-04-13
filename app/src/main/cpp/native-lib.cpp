#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jelly_thor_ndkmd5_SignUtils_signParams(
        JNIEnv *env,
        jclass clazz,
        jstring params) {
    return env->NewStringUTF("签名成功");
}