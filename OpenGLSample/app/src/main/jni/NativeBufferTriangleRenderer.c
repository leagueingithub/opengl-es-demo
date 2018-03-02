//
// Created by King on 2016/5/24.
//

#include <jni.h>
#include "BaseRenderer.h"

GLuint gProgram;
GLuint vboId[2];      //顶点绑定对象id指针（视为数组）  vertex-bind-object id,as array
GLuint vaoId[1];

const char* vShaderStr =
        "#version 300 es\n"
                "layout(location = 0) in vec4 vPosition;\n"
                "layout(location = 1) in vec4 aColor;\n"
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

//顶点数据、属性
// vertices data and attributes
const GLfloat vertices[] = {

        0.1f, 0.5f, 0.0f,       //第一个点      first point     x,y,z
        1.0f, 0.5f, 0.5f, 1.0f, //第一个颜色    first color     r,g,b,a

        -0.3f, -0.4f, 0.0f,     //第二个点       second point
        0.5f, 1.0f, 0.5f, 1.0f, //第二个颜色     second color

        0.5f, 0.4f, 0.0f,       //第三个点      third point
        0.5f, 0.5f, 1.0f, 1.0f  //第三个颜色     third color

};

//顶点索引数组
//vertices index array
const GLushort indices[3] = {0, 1, 2};

GLboolean setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    gProgram = createProgram(vShaderStr, fShaderStr);
    if (!gProgram) {
        LOGE("Could not create program.");
        return GL_FALSE;
    }

    LOGI("glGenBuffers 2 buffers");
    //生成两个顶点缓存，返回id保存到vboId中
    //generate 2 vertex buffer array and saved pointer into vboId
    glGenBuffers(2, vboId);
    checkGlError("glGenBuffers");

    LOGI("glBindBuffer 0");
    //绑定到第一个缓存数组
    //bind to first array
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    //缓存顶点数据到第一个数组
    //buffered data into first array
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    LOGI("glBindBuffer 1");
    //绑定到第二个缓存数组，用于存储顶点索引信息
    //bind to second array for vertices index info
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    //缓存顶点索引数据到第二个数组
    //buffered vertices index data into second array
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, vaoId);

    glBindVertexArray(vaoId[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, (const void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, (const void*)(3* sizeof(GLfloat)));

    glBindVertexArray(0);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return GL_TRUE;
}



void renderFrame() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glBindVertexArray(vaoId[0]);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (const void*) 0);

    glBindVertexArray(0);

}

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeBufferTriangleRenderer_init(JNIEnv *env, jclass type, jint width,  jint height) {

// TODO
    setupGraphics(width, height);

}


JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeBufferTriangleRenderer_step(JNIEnv *env,
                                                                                     jclass type) {

    // TODO
    renderFrame();

}