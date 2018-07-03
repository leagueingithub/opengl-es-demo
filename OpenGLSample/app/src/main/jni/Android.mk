LOCAL_PATH := $(call my-dir)
GLM_FILE_PATH := $(shell find "$(LOCAL_PATH)/glm/")
GLM_SOURCE_FILE_LIST := $(filter %.cpp %.inl %.hpp %.h, $(GLM_FILE_PATH))
GLM_SOURCE_FILES := $(GLM_SOURCE_FILE_LIST:$(LOCAL_PATH)/%=%)

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

#### create libtransformations.so file ####

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(GLM_FILE_PATH)
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3 -lm
LOCAL_MODULE    := transformations

LOCAL_SRC_FILES := \
$(GLM_SOURCE_FILES) \
BaseRenderer.c \
TransformationsRenderer.cpp

include $(BUILD_SHARED_LIBRARY)


#### create libtransform3d.so file ####

include $(CLEAR_VARS)

LOCAL_MODULE    := transform3d
LOCAL_SRC_FILES := \
$(GLM_SOURCE_FILES) \
BaseRenderer.c \
transform3d.cpp \
Transform3dRenderer.cpp

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lGLESv3 -lm
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)