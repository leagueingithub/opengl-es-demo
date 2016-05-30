package com.league.king.openglsample.native_renderer;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by King on 2016/5/24.
 */
public class NativeBufferTriangleRenderer implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("bufferTriangleRenderer");
    }

    public static native void init(int width, int height);

    public static native void step();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        init(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        step();
    }
}
