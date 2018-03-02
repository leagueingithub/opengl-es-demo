package com.league.king.openglsample.util;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.content.PermissionChecker;

import com.league.king.openglsample.R;

import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Created by King on 2016/7/11.
 * <p>
 * Android 6.0 权限工具
 */
public class PermissionUtil {

    /**
     * Created by King on 2016/7/11.
     */
    public static class TitRequestPermissionActivity extends Activity {

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);

            setContentView(R.layout.request_permission);

            String[] permissions;

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                Intent getIntent = getIntent();
                if (getIntent.hasExtra("permissions")) {
                    permissions = getIntent.getStringArrayExtra("permissions");
                    if (permissions == null || permissions.length == 0) {

                        finish();
                        return;
                    }
                } else {

                    finish();
                    return;
                }

                requestPermissions(permissions, 0);
            }
        }

        @Override
        public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

                int code = Arrays.hashCode(permissions);
                OnPermissionActionListener listener = PermissionUtil.getListener(code);

                Map<String, Boolean> resultMap = new LinkedHashMap<>();

                if (permissions.length != 0) {

                    for (int i = 0; i < permissions.length; i++) {
                        String permission = permissions[i];
                        int permissionCheck = grantResults[i];

                        //用户拒绝
                        if (permissionCheck != PackageManager.PERMISSION_GRANTED) {

                            //重复权限不再判断
                            if (resultMap.containsKey(permission))
                                continue;

                            if (shouldShowRequestPermissionRationale(permission)) {
                                //用户此次拒绝此权限
                                resultMap.put(permission, true);
                            } else {
                                //用户永久拒绝此权限
                                resultMap.put(permission, false);
                            }
                        } else if (permissionCheck == PackageManager.PERMISSION_GRANTED) {//已授权监听
                            listener.onAcceptPermissions(permission);
                        }


                    }

                    if (resultMap.size() > 0) {
                        String[] denyPermissions = resultMap.keySet().toArray(new String[1]);
                        Boolean[] canRequestAgain = resultMap.values().toArray(new Boolean[1]);

                        if (listener != null) {
                            listener.onDenyPermissions(denyPermissions, canRequestAgain);
                        }
                    } else {

                        if (listener != null) {
                            listener.onDenyPermissions(null, null);
                        }
                    }
                }

            }
            finish();
        }

    }

    /**
     * 拒绝监听器
     * 返回数组是被拒绝的权限以及能否再次申请此权限的bool值
     * null则是用户全通过。
     */
    public interface OnPermissionActionListener {
        void onDenyPermissions(String[] permission, Boolean[] canRequestAgain);

        void onAcceptPermissions(String permission);
    }


    private static Map<Integer, OnPermissionActionListener> requestPermissionResultListenerMap = new HashMap<>();

    /**
     * 获取监听器
     *
     * @param code
     * @return
     */
    public static OnPermissionActionListener getListener(int code) {

        if (requestPermissionResultListenerMap.containsKey(code)) {
            return requestPermissionResultListenerMap.remove(code);
        }
        return null;
    }

    /**
     * 判断用户是否给予此权限
     *
     * @param permission 权限，详情可以看{@link android.Manifest.permission}
     * @return 是否拥有此权限
     */
    public static boolean hasPermission(Context context, String permission) {

        boolean result = false;
        if (context != null && permission != null && !permission.equals("")) {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                int permissionCheck = context.checkSelfPermission(permission);

                result = permissionCheck == PackageManager.PERMISSION_GRANTED;
            } else {//android 23 以下权限检测方式。

                int permissionCheck = PermissionChecker.checkSelfPermission(context, permission);
                result = permissionCheck == PermissionChecker.PERMISSION_GRANTED;
            }
        }

        return result;
    }

    /**
     * 向用户请求权限
     *
     * @param permission 权限名称
     * @param listener   被拒绝权限返回监听
     */
    public static void needPermission(Context context, String permission, OnPermissionActionListener listener) {

        if (context == null || permission == null || permission.equals(""))
            return;

        //已拥有此权限
        if (hasPermission(context, permission))
            return;

        needPermissions(context, new String[]{permission}, listener);

    }

    /**
     * 向用户请求多个权限
     *
     * @param permissions 权限数组
     * @param listener    被拒绝权限返回监听
     */
    public static void needPermissions(Context context, String[] permissions, OnPermissionActionListener listener) {

        if (context == null || permissions == null || permissions.length == 0)
            return;

        int code = Arrays.hashCode(permissions);
        if (requestPermissionResultListenerMap.containsKey(code))
            return;

        requestPermissionResultListenerMap.put(code, listener);
        requestPermission(context, permissions);
    }

    /**
     * 请求权限方法
     *
     * @param permissions 权限数组
     */
    private static void requestPermission(Context context, String[] permissions) {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            Intent intent = new Intent(context, TitRequestPermissionActivity.class);
            intent.putExtra("permissions", permissions);
            context.startActivity(intent);
        }

    }


}
