//
// Created by league on 18-2-11.
//
#include<jni.h>
#ifndef OPENGLSAMPLE_TEXTURERENDERER_H
#define OPENGLSAMPLE_TEXTURERENDERER_H

#ifndef _Included_com_league_king_openglsample_native_1renderer_NativeTextureRenderer
#define _Included_com_league_king_openglsample_native_1renderer_NativeTextureRenderer
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTextureRenderer_init(JNIEnv *env,
                                                                                     jclass clazz,
                                                                                     jobject assetManager,
                                                                                     jstring fileName,
                                                                                     jint width,
                                                                                     jint height);

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTextureRenderer_step(JNIEnv *env, jclass clazz);

#ifdef __cplusplus
}
#endif
#endif
#endif //OPENGLSAMPLE_TEXTURERENDERER_H
