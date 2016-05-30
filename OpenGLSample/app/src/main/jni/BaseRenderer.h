//
// Created by King on 2016/5/24.
//
#include <android/log.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#ifndef OPENGL_SAMPLE_BASERENDERER_H
#define OPENGL_SAMPLE_BASERENDERER_H

#endif //OPENGL_SAMPLE_BASERENDERER_H

#define  LOG_TAG    "base_renderer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);

GLuint loadShader(GLenum shaderType, const char* pSource);

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);