LOCAL_PATH := $(call my-dir)

#### create libgl2jni.so file ####

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3
LOCAL_MODULE    := gl2jni
LOCAL_SRC_FILES := \
BaseRenderer.c \
native_renderer.c
include $(BUILD_SHARED_LIBRARY)

#### create libbufferTriangleRenderer.so file ####

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3
LOCAL_MODULE    := bufferTriangleRenderer
LOCAL_SRC_FILES := \
BaseRenderer.c \
NativeBufferTriangleRenderer.c
include $(BUILD_SHARED_LIBRARY)

#### create libmipmap2DRenderer.so file ####

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3
LOCAL_MODULE    := mipmap2DRenderer
LOCAL_SRC_FILES := \
BaseRenderer.c \
MipMap2DRenderer.c
include $(BUILD_SHARED_LIBRARY)


#### create libparticleSystem.so file ####

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3
LOCAL_MODULE    := particleSystem
LOCAL_SRC_FILES := \
BaseRenderer.c \
ParticleSystemRenderer.c
include $(BUILD_SHARED_LIBRARY)

#### create libparticleSystemTransformFeedback.so file ####

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3
LOCAL_MODULE    := particleSystemTransformFeedback
LOCAL_SRC_FILES := \
BaseRenderer.c \
ParticleSystemTransformFeedback/Noise3D.c \
ParticleSystemTransformFeedback/ParticleSystemTransformFeedback.c
include $(BUILD_SHARED_LIBRARY)

#### create libtextureRenderer.so file ####

include $(CLEAR_VARS)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3
LOCAL_MODULE    := textureRenderer
LOCAL_SRC_FILES := \
BaseRenderer.c \
TextureRenderer.cpp
include $(BUILD_SHARED_LIBRARY)