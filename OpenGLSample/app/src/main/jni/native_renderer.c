#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BaseRenderer.h"


GLuint gProgram;
GLuint gvPositionHandle;
GLuint gcPositionHandle;


const char* vShaderStr =
        "#version 300 es\n"
        "in vec4 vPosition;\n"
        "in vec4 aColor;\n"
        "out vec4 vColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vPosition;\n"
        "   vColor = aColor;\n"
        "}\n";

const char* fShaderStr =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 vColor;\n"
        "out vec4 fragColor;\n"
        "void main()\n"
        "{\n"
        "  fragColor = vec4 ( vColor );\n"
        "}\n";



GLboolean setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(vShaderStr, fShaderStr);
    if (!gProgram) {
        LOGE("Could not create program.");
        return GL_FALSE;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
         gvPositionHandle);

    gcPositionHandle = glGetAttribLocation(gProgram, "aColor");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"aColor\") = %d\n",
         gcPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return GL_TRUE;
}

const GLfloat gTriangleVertices[] = { 0.5f, 0.5f, 0.0f,
                                      0.0f, 0.0f, 0.0f,
                                      -0.5f, 0.2f, 0.0f};

const GLfloat gColors[] = { 0.1f, 0.5f, 0.0f, 1.0f,
                            0.8f, 0.1f, 0.5f, 1.0f,
                            0.3f, 0.7f, 0.4f, 1.0f};

void renderFrame() {

    static float grey;
    grey += 0.01f;
    if (grey > 0.5f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    glVertexAttribPointer(gcPositionHandle, 4, GL_FLOAT, GL_FALSE, 0, gColors);

    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    glEnableVertexAttribArray(gcPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}

//extern "C" {
//JNIEXPORT void JNICALL Java_com_league_king_openglsample_native_1renderer_NativeTriangleRenderer_init(JNIEnv * env, jobject obj,  jint width, jint height);
//JNIEXPORT void JNICALL Java_com_league_king_openglsample_native_1renderer_NativeTriangleRenderer_step(JNIEnv * env, jobject obj);
//};

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTriangleRenderer_init(JNIEnv *env,
                                                                               jclass clazz, jint width,
                                                                               jint height) {

    // TODO
    setupGraphics(width, height);



}

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTriangleRenderer_step(JNIEnv *env,
                                                                               jclass clazz) {

    // TODO

    renderFrame();
}
