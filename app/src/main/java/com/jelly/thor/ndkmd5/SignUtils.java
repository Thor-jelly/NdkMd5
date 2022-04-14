package com.jelly.thor.ndkmd5;

import android.content.Context;

/**
 * 类描述：加密签名工具类 <br/>
 * 创建人：吴冬冬<br/>
 * 创建时间：2022/4/13 22:43 <br/>
 */
public class SignUtils {
    static {
        System.loadLibrary("ndkmd5");
    }
    //签名参数，方法不能被混淆，其他人可以通过hook到你调用的方法，然后可以自己写一个项目把.so库拿过去用
    //那怎么解决？通过签名校验
    public static native String signParams(String params);

    //签名校验方法，只允许自己的app可以使用自己的.so库
    //在application中调用
    public static native String signVerify(Context context);
}
