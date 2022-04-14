#include <jni.h>
#include <string>
#include <android/log.h>
#include "md5.h"

//额外的加密添加规则
static const char *EXTRE_SING = "WDD";
//签名校验
static int is_verify = 0;//是否签名
static char *PACKAGE_NAME = "com.jelly.thor.ndkmd5";//app的包名
static char *APP_SIGN = "308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3231313032343038353233395a180f32303531313031373038353233395a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a0282010100a29b7f6ec64281555fa8ebe6c1a3e8d40806ef295a5f1b265c00ab8392e4a5e81e75287c606484f2ef756028463ab132b82677e7d5bcbab42c92ec10f2fce1ce939b517c9d4d1edf6c87dab5ce3c7559521cefd8257ce3f4dd0a37665795bf8a7bdc830987efb239491f410ff0e1744e6ce33da74c995b39958a1bb138ba0b1e2ed1fca5265f512317c70c92213f5f58d8421a0926c516384ec60a477e7b58bc0991ee50d92ecb179aa57e934570d119bf6407c2fc1d7fae96082da9e1e1a8c415cac515cb9102078373cf50cc13e2dc1cffacc1e42cd9cc6be9cb3375df03aa627ec10b57d19e8a4f3d605de37d794d773f30051104ca0bde4a30d4099413210203010001300d06092a864886f70d010105050003820101006f3fd16c928042f462e0bc16f54ccd1b386bc11572038cbaa0fdf30c3fc06c0a289f3c10f1676d6305c4c7815abd4e02cab845a54810c8622de6c9daf14329f70ef1f02ecc39f56a5e8667110986a2a74b267c1b9f337a62b5170873c2bab5f25faa36cfd8b55c9f8a4a918b5b7a73bfec12d2cb4183046c40d088727a31742cb123ec19cb0c88e0090e2239be167a67db5d27a28765a589f114e1f55f565154a5493daf0067470574489318251c6d1e18f6255928160e63776de6f68cd9805ec4f680d59b2ce6f163f03a32d816687a5c1aad3926f88672f49bb962c06934f311dc4c9e7eb6195d806a28aaade82e86ae34fef7c3b993af41300aec687201b3";//app的签名

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jelly_thor_ndkmd5_SignUtils_signParams(
        JNIEnv *env,
        jclass clazz,
        jstring params) {
    if (is_verify == 0) {
        return env->NewStringUTF("错误的签名！");
    }

    const char *p = env->GetStringUTFChars(params, 0);
    //MD5签名
    //参数过来我可以来点特殊规则拼接 防止被破解
    std::string sign_str(p);
    sign_str.insert(0, EXTRE_SING);

    //md5加密 C++ 和 Java一样的，唯一不同就是需要自己回收内存
    auto *md5Ctx = new MD5_CTX();
    MD5Init(md5Ctx);
    MD5Update(md5Ctx, (unsigned char *) sign_str.c_str(), sign_str.length());
    unsigned char digest[16] = {0};
    MD5Final(md5Ctx, digest);

    //生成32位字符串
    char md5_str[33] = {0};
    for (unsigned char i : digest) {
        //不足情况下补0
        sprintf(md5_str, "%s%02x", md5_str, i);
    }

    //释放资源
    env->ReleaseStringUTFChars(params, p);

    return env->NewStringUTF(md5_str);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jelly_thor_ndkmd5_SignUtils_signVerify(
        JNIEnv *env,
        jclass clazz,
        jobject context) {
    //通过c代码调用Java代码
    /*
     * try {
            PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            String signatureStr = signatures[0].toCharsString();
            Log.d("123===", "activity中获取到签名：" + signatureStr);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
     */
    //1.获取app包名
    //c调用Java从内往外走
    //1.1 第一个方法 getPackageName
    //1.1.1  通过env->CallObjectMethod 调用方法  然后慢慢反推 需要的参数
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_mid = env->GetMethodID(j_clz, "getPackageName",
                                       "()Ljava/lang/String;");//第三个参数()表示无参 后面的表示返回值可以查
    jstring j_package_name = static_cast<jstring>(env->CallObjectMethod(context, j_mid));
    //2.比对包名是否一样
    const char *c_package_name = env->GetStringUTFChars(j_package_name, NULL);
    if (strcmp(c_package_name, PACKAGE_NAME) != 0) {
        return env->NewStringUTF("包名错误！");
    }
    __android_log_print(ANDROID_LOG_ERROR, "JNI_TAG", "包名一致：%s", c_package_name);
    //3.获取签名
    //4.比对签名是否一样
    return env->NewStringUTF("签名校验正确！");
}