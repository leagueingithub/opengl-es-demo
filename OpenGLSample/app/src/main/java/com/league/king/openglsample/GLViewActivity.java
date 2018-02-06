package com.league.king.openglsample;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.league.king.openglsample.native_renderer.NativeBufferTriangleRenderer;
import com.league.king.openglsample.native_renderer.NativeMipMap2DRenderer;
import com.league.king.openglsample.native_renderer.NativeParticleSystemRenderer;
import com.league.king.openglsample.native_renderer.NativeTriangleRenderer;
import com.league.king.openglsample.renders.TriangleRenderer;

/**
 * Created by King on 2016/5/18.
 *
 */
public class GLViewActivity extends Activity {

    public final static String EXTRA_WHICH_RENDERER = "whichRenderer";

    private final static int RENDERER_TRIANGLE = 0;
    private final static int RENDERER_NATIVE_TRIANGLE = 1;
    private final static int RENDERER_NATIVE_BUFFER_TRIANGLE = 2;
    private final static int RENDERER_MIPMAP2D = 3;
    private final static int RENDERER_PARTICLE_SYSTEM = 4;

    private GLSurfaceView   mGlSurfaceView;
    private int renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        renderer = getIntent().getIntExtra(EXTRA_WHICH_RENDERER, 0);
        setContentView(R.layout.activity_glview);

        mGlSurfaceView = (GLSurfaceView) findViewById(R.id.glsurfaceview);

        /**
         * 启动深度检测
         * turn on depth
         */
//        mGlSurfaceView.setEGLConfigChooser(true);
        /**
         * 设置成主动连续渲染模式
         * set render continuously
         */
//        mGlSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        /**
         * 设置View窗口在宿主窗口上方
         * set surfaceView'window above it's parent
         */
//        mGlSurfaceView.setZOrderOnTop(true);

        /**
         * 允许透明，以便可以看到surfaceView窗口下的内容
         * set window translucent, it can see content below surface's window
         */
//        mGlSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

        if (detectOpenGLES30()) {

            mGlSurfaceView.setEGLContextClientVersion(3);

            GLSurfaceView.Renderer r = null;
            switch (renderer) {
                case RENDERER_TRIANGLE: {
                    r = new TriangleRenderer();
                    break;
                }
                case RENDERER_NATIVE_TRIANGLE: {
                    r = new NativeTriangleRenderer();
                    break;
                }
                case RENDERER_NATIVE_BUFFER_TRIANGLE: {
                    r = new NativeBufferTriangleRenderer();
                    break;
                }
                case RENDERER_MIPMAP2D: {
                    r = new NativeMipMap2DRenderer();
                    break;
                }
                case RENDERER_PARTICLE_SYSTEM: {
                    r = new NativeParticleSystemRenderer(this);
                    break;
                }
            }
            mGlSurfaceView.setRenderer(r);
        }


    }

    /**
     * 判断设备是否支持opengl es 3.0
     * checked opengl es 3.0 enable or not
     * @return
     */
    private boolean detectOpenGLES30()
    {
        ActivityManager am =
                ( ActivityManager ) getSystemService ( Context.ACTIVITY_SERVICE );
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return ( info.reqGlEsVersion >= 0x30000 );
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGlSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGlSurfaceView.onResume();
    }
}
