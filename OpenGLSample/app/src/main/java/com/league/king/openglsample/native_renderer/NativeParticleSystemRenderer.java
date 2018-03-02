package com.league.king.openglsample.native_renderer;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by league on 18-2-5.
 */

public class NativeParticleSystemRenderer implements GLSurfaceView.Renderer {

    private final static String TGAFILE = "smoke.tga";

    private AssetManager assetManager;

    static {
        System.loadLibrary("particleSystem");
    }

    public static native void init(AssetManager manager, String fileName, int width, int height);

    public static native void step();

    public NativeParticleSystemRenderer() {
        assetManager = null;
    }

    public NativeParticleSystemRenderer(Context context) {
        assetManager = context.getAssets();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        init(assetManager, TGAFILE, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        step();
    }
}
