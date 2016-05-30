package com.league.king.openglsample.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * Created by King on 2016/5/19.
 *
 */
public class BufferUtil {

    public static FloatBuffer asFloatBuffer(float[] data) {

        if (data == null)
            return null;

        FloatBuffer result = ByteBuffer.allocateDirect(data.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        result.put(data).position(0);

        return result;
    }
}
