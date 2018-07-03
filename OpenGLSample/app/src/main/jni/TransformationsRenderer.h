//
// Created by league on 18-3-9.
//
#include<jni.h>

#ifndef OPENGLSAMPLE_TRANSFORMATIONSRENDERER_H
#define OPENGLSAMPLE_TRANSFORMATIONSRENDERER_H


#ifndef _Included_com_league_king_openglsample_native_1renderer_NativeTransformationsRenderer
#define _Included_com_league_king_openglsample_native_1renderer_NativeTransformationsRenderer
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTransformationsRenderer_init(JNIEnv *env,
                                                                                     jclass clazz,
                                                                                     jobject assetManager,
                                                                                     jstring fileName,
                                                                                     jint width,
                                                                                     jint height);

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeTransformationsRenderer_step(JNIEnv *env, jclass clazz);

#ifdef __cplusplus
}
#endif
#endif

#endif //OPENGLSAMPLE_TRANSFORMATIONSRENDERER_H
