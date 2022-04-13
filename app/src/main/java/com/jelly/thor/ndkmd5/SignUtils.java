package com.jelly.thor.ndkmd5;

/**
 * 类描述：加密签名工具类 <br/>
 * 创建人：吴冬冬<br/>
 * 创建时间：2022/4/13 22:43 <br/>
 */
public class SignUtils {
    static {
        System.loadLibrary("ndkmd5");
    }
    //签名参数
    public static native String signParams(String params);
}
