package com.league.king.openglsample.native_renderer;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by league on 18-2-5.
 */

public class NativeMipMap2DRenderer implements GLSurfaceView.Renderer {

    static {
        System.loadLibrary("mipmap2DRenderer");
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
