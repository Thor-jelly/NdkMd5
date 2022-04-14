package com.jelly.thor.ndkmd5;

import android.app.Application;

/**
 * 创建人：吴冬冬<br/>
 * 创建时间：2022/4/15 00:14 <br/>
 */
public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        SignUtils.signVerify(this);
    }
}
