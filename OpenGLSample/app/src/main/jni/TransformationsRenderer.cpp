//
// Created by league on 18-2-9.
//
#define STB_IMAGE_IMPLEMENTATION
#include "TransformationsRenderer.h"
#include "stb_image.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <sys/time.h>
#include "BaseRenderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

////////////// Global variable /////////////
int screen_width, screen_height;
GLuint g_Program;       //全局实例
GLuint g_vertexArrayObject;
GLuint g_vertexBuffer;
GLuint g_elementBuffer;
GLuint g_texture;       //纹理

float userTime;
float deltaTime;
long int prevTime;

float radius = 0.5f;
float rotateAngle;
float transX, transY;

void loadTexture(const char* fileName) {

    int width, height, nChannels;

    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* imageData = stbi_load(fileName, &width, &height, &nChannels, 0);

    if (imageData) {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(imageData);
    } else{
        LOGE("读取纹理图片失败");
    }
}

/**
 * 初始化
 * @param ioContext 上下文指针（此处传入的是AssetManager）
 * @param fileName  文件名
 * @param width     窗口宽度
 * @param height    窗口高度
 */
void Init(void* ioContext, const char* fileName, int width, int height) {

    screen_width = width;
    screen_height = height;
    rotateAngle = 0.0f;
    transX = radius;
    transY = 0.0f;
    deltaTime = userTime = 0;
    prevTime = 0;

    const char *vShaderStr =
            "#version 300 es                                                        \n"
                    "layout (location = 0) in vec3 a_position;                      \n"
                    "layout (location = 1) in vec3 a_color;                         \n"
                    "layout (location = 2) in vec2 a_texture;                       \n"
                    "out vec3 pcolor;                                               \n"
                    "out vec2 texcoord;                                             \n"
                    "uniform mat4 transform;                                        \n"
                    "void main()                                                    \n"
                    "{                                                              \n"
                    "   gl_Position = transform * vec4(a_position, 1.0);            \n"
                    "   pcolor = a_color;                                           \n"
                    "   texcoord = a_texture;                                       \n"
                    "}";

    const char *fShaderStr =
            "#version 300 es                                        \n"
                    "in vec3 pcolor;                                \n"
                    "in vec2 texcoord;                              \n"
                    "                                               \n"
                    "uniform vec3 blendColor;                       \n"
                    "                                               \n"
                    "uniform sampler2D tex;                         \n"
                    "out vec4 FragColor;                            \n"
                    "void main()                                    \n"
                    "{                                              \n"
                    "   FragColor = texture(tex, texcoord);         \n"
                    "}";

    g_Program = createProgram(vShaderStr, fShaderStr);

    loadTexture(fileName);

    //顶点
    GLfloat vertices[] = {
            -0.5f,  0.5f,   0.0f,       1.0f, 0.0f, 0.0f,       0.0f, 1.0f,
            -0.5f,  -0.5f,  0.0f,       0.0f, 1.0f, 0.0f,       0.0f, 0.0f,
            0.5f,   0.5f,   0.0f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
            0.5f,   -0.5f,  0.0f,       1.0f, 1.0f, 0.0f,       1.0f, 0.0f
    };

    //顶点数组
    GLushort indices[] = {
            0, 1, 2, 3
    };

    //创建顶点列表对象并绑定
    glGenVertexArrays(1, &g_vertexArrayObject);
    glBindVertexArray(g_vertexArrayObject);

    //绑定顶点数据
    glGenBuffers(1, &g_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //绑定顶点索引数据
    glGenBuffers(1, &g_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //配置顶点信息
    /**
     * 参数说明
     * 1-- 要配置的顶点属性
     * 2-- 指定顶点属性的大小(顶点属性是一个vec3，它由3个值组成，所以大小是3)
     * 3-- 指定数据的类型，这里是GL_FLOAT
     * 4-- 是否希望数据被标准化(Normalize)。如果设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
     * 5-- 步长
     * 6-- 位置数据在缓冲中起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (void*)(6* sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //解绑
    glBindVertexArray(0);
}

/**
 * 绘制函数
 */
void Draw() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, screen_width, screen_height);

    glUseProgram(g_Program);

    //GLint blendColorLocation = glGetUniformLocation(g_Program, "blendColor");
    //glUniform3f(blendColorLocation, 0.5f, 0.5f, 0.5f);

    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(transX, transY, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    GLint transform = glGetUniformLocation(g_Program, "transform");
    glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));


    glBindTexture(GL_TEXTURE_2D, g_texture);
    glBindVertexArray(g_vertexArrayObject);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
}

/**
 * 更新函数
 */
void Update() {

    userTime += deltaTime;

    if (userTime >= 4.0f)
    {
        userTime = 0.0f;
        transX = radius;
        transY = 0.0f;
        rotateAngle = 0.0f;
    }
    else
    {
        float percent = userTime / 4.0f;
        transX = radius * (float)cos(percent * 2 * M_PI);
        transY = radius * (float)sin(percent * 2 * M_PI);
        rotateAngle = percent * 4 * 360;
    }


}

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTransformationsRenderer_init(JNIEnv *env,
                                                                              jclass clazz, jobject assetManager,
                                                                              jstring fileName, jint width, jint height) {

    AAssetManager *mgr;
    if (assetManager != NULL) {
        mgr = AAssetManager_fromJava(env, assetManager);
    } else{
        mgr = NULL;
    }

    const char *fn = env->GetStringUTFChars(fileName, 0);
//    char *f = nullptr;
//    f = const_cast<char *>(fn);

    Init(mgr, fn, width, height);

    env->ReleaseStringUTFChars(fileName, fn);
//    delete (f);
}

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTransformationsRenderer_step(JNIEnv *env,
                                                                              jclass clazz) {


    struct timeval tv;
    gettimeofday(&tv, NULL);
    long int current = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    //LOGE ( "calc -> current time is %lld, prevTime is %lld", current, prevTime );

    if (prevTime == 0) {
        deltaTime = 0;
        prevTime = current;
    } else{
        deltaTime = (current - prevTime) / 1000.0f;
        prevTime = current;
    }

    Draw();
    Update();

}
