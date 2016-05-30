package com.league.king.openglsample.renders;

import android.opengl.GLES30;

import com.league.king.openglsample.util.BufferUtil;

import javax.microedition.khronos.opengles.GL10;

/**
 * Created by King on 2016/5/19.
 *
 */
public class TriangleRenderer extends BaseRenderer {

    private float[] mVertexData = {

            0.5f, 0.5f, 0.0f,
            0.0f, 0.0f, 0.0f,
            -0.5f, 0.2f, 0.0f

    };


    @Override
    public void onDrawFrame(GL10 gl) {

        GLES30.glViewport(0, 0, mWidth, mHeight);

        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT);

        GLES30.glUseProgram(mProgramObject);

        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 0, BufferUtil.asFloatBuffer(mVertexData));
        GLES30.glEnableVertexAttribArray(0);

        GLES30.glBlendColor(0.0f, 1.0f, 0.8f, 1.0f);
        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, 3);

    }
}
