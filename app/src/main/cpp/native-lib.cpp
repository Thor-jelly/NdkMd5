#include <jni.h>
#include <string>
#include "md5.h"

static const char *EXTRE_SING = "WDD";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jelly_thor_ndkmd5_SignUtils_signParams(
        JNIEnv *env,
        jclass clazz,
        jstring params) {
    const char *p = env->GetStringUTFChars(params, 0);
    //MD5签名
    //参数过来我可以来点特殊规则拼接 防止被破解
    std::string sign_str(p);
    sign_str.insert(0, EXTRE_SING);

    //md5加密 C++ 和 Java一样的，唯一不同就是需要自己回收内存
    auto *md5Ctx = new MD5_CTX();
    MD5Init(md5Ctx);
    MD5Update(md5Ctx, (unsigned char *) sign_str.c_str(), sign_str.length());
    unsigned char digest[16]={0};
    MD5Final(md5Ctx, digest);

    //生成32位字符串
    char md5_str[33] = {0};
    for (unsigned char i : digest){
        //不足情况下补0
        sprintf(md5_str, "%s%02x", md5_str, i);
    }

    //释放资源
    env->ReleaseStringUTFChars(params, p);

    return env->NewStringUTF(md5_str);
}