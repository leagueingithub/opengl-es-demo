package com.league.king.openglsample.renders;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by King on 2016/5/18.
 *
 */
public abstract class BaseRenderer implements GLSurfaceView.Renderer {

    protected int mProgramObject;
    protected int mWidth;
    protected int mHeight;

    private int LoadShader(int type, String shaderSrc) {

        int shader;
        int[] compiled = new int[1];

        shader = GLES30.glCreateShader(type);

        if (shader == 0) {
            return 0;
        }

        GLES30.glShaderSource(shader, shaderSrc);

        GLES30.glCompileShader(shader);

        GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compiled, 0);

        if (compiled[0] == 0) {
            GLES30.glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        String vShaderStr =
                "#version 300 es 			  \n"
                        +   "in vec4 vPosition;           \n"
                        + "void main()                  \n"
                        + "{                            \n"
                        + "   gl_Position = vPosition;  \n"
                        + "}                            \n";

        String fShaderStr =
                "#version 300 es		 			          	\n"
                        + "precision mediump float;					  	\n"
                        + "out vec4 fragColor;	 			 		  	\n"
                        + "void main()                                  \n"
                        + "{                                            \n"
                        + "  fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );	\n"
                        + "}                                            \n";


        int vertexShader;
        int fragmentShader;
        int programObject;
        int [] linked = new int[1];

        vertexShader = LoadShader(GLES30.GL_VERTEX_SHADER, vShaderStr);
        fragmentShader = LoadShader(GLES30.GL_FRAGMENT_SHADER, fShaderStr);

        programObject = GLES30.glCreateProgram();

        if (programObject == 0) {
            return;
        }

        GLES30.glAttachShader(programObject, vertexShader);
        GLES30.glAttachShader(programObject, fragmentShader);

        GLES30.glBindAttribLocation(programObject, 0, "vPosition");

        GLES30.glLinkProgram(programObject);

        GLES30.glGetProgramiv(programObject, GLES30.GL_LINK_STATUS, linked, 0);

        if (linked[0] == GLES30.GL_ZERO) {
            GLES30.glDeleteProgram(programObject);
            return;
        }

        mProgramObject = programObject;

        GLES30.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

        mWidth = width;
        mHeight = height;

    }

}
