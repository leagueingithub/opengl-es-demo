// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// ParticleSystem.c
//
//    This is an example that demonstrates rendering a particle system
//    using a vertex shader and point sprites.
//
#include <stdlib.h>
#include <math.h>
#include <GLES3/gl3.h>
#include <jni.h>
#include <stdio.h>
#include "BaseRenderer.h"
#include <android/asset_manager_jni.h>
#include <sys/time.h>

#define NUM_PARTICLES   1000
#define PARTICLE_SIZE   7

#define ATTRIBUTE_LIFETIME_LOCATION       0
#define ATTRIBUTE_STARTPOSITION_LOCATION  1
#define ATTRIBUTE_ENDPOSITION_LOCATION    2

typedef struct
{
    // Handle to a program object
    GLuint programObject;

    // Uniform location
    GLint timeLoc;
    GLint colorLoc;
    GLint centerPositionLoc;
    GLint samplerLoc;

    // Texture handle
    GLuint textureId;

    // Particle vertex data
    float particleData[ NUM_PARTICLES * PARTICLE_SIZE ];

    // Current time
    float time;

} UserData;

typedef struct
{
    void *data;
    void *platformData;
    int width;
    int height;
} Context;

Context *context;
float deltaTime;
long int prevTime;

#pragma pack(push, x1)
#pragma pack(1)
typedef struct
{
    unsigned char  IdSize,
            MapType,
            ImageType;
    unsigned short PaletteStart,
            PaletteSize;
    unsigned char  PaletteEntryDepth;
    unsigned short X,
            Y,
            Width,
            Height;
    unsigned char  ColorDepth,
            Descriptor;

} TGA_HEADER;
#pragma pack(pop, x1)

///
// Load texture from disk
//
GLuint LoadTexture ( void *ioContext, char *fileName )
{
    int width,
            height;
    char *buffer = esLoadTGA ( ioContext, fileName, &width, &height );
    GLuint texId;

    if ( buffer == NULL )
    {
        LOGE ( "Error loading (%s) image.\n", fileName );
        return 0;
    }

    glGenTextures ( 1, &texId );
    glBindTexture ( GL_TEXTURE_2D, texId );

    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    free ( buffer );

    return texId;
}


///
// Initialize the shader and program object
//
int Init ( void* assetManager, char* fileName, int width, int height )
{

    context = malloc(sizeof(Context));
    context->width = width;
    context->height = height;
    context->data = malloc ( sizeof ( UserData ) );
    if (assetManager == NULL) {
        context->platformData = NULL;
    } else{
        context->platformData = assetManager;
    }
    UserData *userData = context->data;
    int i;

    char vShaderStr[] =
            "#version 300 es                                      \n"
                    "uniform float u_time;                                \n"
                    "uniform vec3 u_centerPosition;                       \n"
                    "layout(location = 0) in float a_lifetime;            \n"
                    "layout(location = 1) in vec3 a_startPosition;        \n"
                    "layout(location = 2) in vec3 a_endPosition;          \n"
                    "out float v_lifetime;                                \n"
                    "void main()                                          \n"
                    "{                                                    \n"
                    "  if ( u_time <= a_lifetime )                        \n"
                    "  {                                                  \n"
                    "    gl_Position.xyz = a_startPosition +              \n"
                    "                      (u_time * a_endPosition);      \n"
                    "    gl_Position.xyz += u_centerPosition;             \n"
                    "    gl_Position.w = 1.0;                             \n"
                    "  }                                                  \n"
                    "  else                                               \n"
                    "  {                                                  \n"
                    "     gl_Position = vec4( -1000, -1000, 0, 0 );       \n"
                    "  }                                                  \n"
                    "  v_lifetime = 1.0 - ( u_time / a_lifetime );        \n"
                    "  v_lifetime = clamp ( v_lifetime, 0.0, 1.0 );       \n"
                    "  gl_PointSize = ( v_lifetime * v_lifetime ) * 40.0; \n"
                    "}";

    char fShaderStr[] =
            "#version 300 es                                      \n"
                    "precision mediump float;                             \n"
                    "uniform vec4 u_color;                                \n"
                    "in float v_lifetime;                                 \n"
                    "layout(location = 0) out vec4 fragColor;             \n"
                    "uniform sampler2D s_texture;                         \n"
                    "void main()                                          \n"
                    "{                                                    \n"
                    "  vec4 texColor;                                     \n"
                    "  texColor = texture( s_texture, gl_PointCoord );    \n"
                    "  fragColor = vec4( u_color ) * texColor;            \n"
                    "  fragColor.a *= v_lifetime;                         \n"
                    "}                                                    \n";

    // Load the shaders and get a linked program object
    userData->programObject = createProgram ( vShaderStr, fShaderStr );

    // Get the uniform locations
    userData->timeLoc = glGetUniformLocation ( userData->programObject, "u_time" );
    userData->centerPositionLoc = glGetUniformLocation ( userData->programObject, "u_centerPosition" );
    userData->colorLoc = glGetUniformLocation ( userData->programObject, "u_color" );
    userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

    // Fill in particle data array
    srand ( 0 );

    for ( i = 0; i < NUM_PARTICLES; i++ )
    {
        float *particleData = &userData->particleData[i * PARTICLE_SIZE];

        // Lifetime of particle
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 10000.0f );

        // End position of particle
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 5000.0f ) - 1.0f;
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 5000.0f ) - 1.0f;
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 5000.0f ) - 1.0f;

        // Start position of particle
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 40000.0f ) - 0.125f;
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 40000.0f ) - 0.125f;
        ( *particleData++ ) = ( ( float ) ( rand() % 10000 ) / 40000.0f ) - 0.125f;

    }

    // Initialize time to cause reset on first update
    userData->time = 1.0f;

    userData->textureId = LoadTexture ( context->platformData, fileName );

    if ( userData->textureId <= 0 )
    {
        LOGE ( "init fail" );
        return GL_FALSE;
    }
    LOGE ( "init success" );

    return GL_TRUE;
}

///
//  Update time-based variables
//
void Update ( )
{
    UserData *userData = context->data;

    userData->time += deltaTime;

    glUseProgram ( userData->programObject );

    //LOGE ( "update -> time is %f and deltaTime is %f", userData->time, deltaTime );

    if ( userData->time >= 1.0f )
    {
        float centerPos[3];
        float color[4];

        userData->time = 0.0f;

        // Pick a new start location and color
        centerPos[0] = ( ( float ) ( rand() % 10000 ) / 10000.0f ) - 0.5f;
        centerPos[1] = ( ( float ) ( rand() % 10000 ) / 10000.0f ) - 0.5f;
        centerPos[2] = ( ( float ) ( rand() % 10000 ) / 10000.0f ) - 0.5f;

        glUniform3fv ( userData->centerPositionLoc, 1, &centerPos[0] );

        // Random color
        color[0] = ( ( float ) ( rand() % 10000 ) / 20000.0f ) + 0.5f;
        color[1] = ( ( float ) ( rand() % 10000 ) / 20000.0f ) + 0.5f;
        color[2] = ( ( float ) ( rand() % 10000 ) / 20000.0f ) + 0.5f;
        color[3] = 0.5;

        glUniform4fv ( userData->colorLoc, 1, &color[0] );
    }

    // Load uniform time variable
    glUniform1f ( userData->timeLoc, userData->time );
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( )
{
    UserData *userData = context->data;

    // Set the viewport
    glViewport ( 0, 0, context->width, context->height );

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( userData->programObject );

    // Load the vertex attributes
    glVertexAttribPointer ( ATTRIBUTE_LIFETIME_LOCATION, 1, GL_FLOAT,
                            GL_FALSE, PARTICLE_SIZE * sizeof ( GLfloat ),
                            userData->particleData );

    glVertexAttribPointer ( ATTRIBUTE_ENDPOSITION_LOCATION, 3, GL_FLOAT,
                            GL_FALSE, PARTICLE_SIZE * sizeof ( GLfloat ),
                            &userData->particleData[1] );

    glVertexAttribPointer ( ATTRIBUTE_STARTPOSITION_LOCATION, 3, GL_FLOAT,
                            GL_FALSE, PARTICLE_SIZE * sizeof ( GLfloat ),
                            &userData->particleData[4] );


    glEnableVertexAttribArray ( ATTRIBUTE_LIFETIME_LOCATION );
    glEnableVertexAttribArray ( ATTRIBUTE_ENDPOSITION_LOCATION );
    glEnableVertexAttribArray ( ATTRIBUTE_STARTPOSITION_LOCATION );

    // Blend particles
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE );

    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, userData->textureId );

    // Set the sampler texture unit to 0
    glUniform1i ( userData->samplerLoc, 0 );

    glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
}

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeParticleSystemRenderer_init(JNIEnv *env,
                                                                                     jclass clazz, jobject assetManager, jstring fileName, jint width,
                                                                                     jint height) {


    AAssetManager *mgr;
    if (assetManager != NULL) {
        mgr = AAssetManager_fromJava(env, assetManager);
    } else{
        mgr = NULL;
    }

    const char *fn = (*env)->GetStringUTFChars(env, fileName, 0);

    Init(mgr, fn, width, height);

    (*env)->ReleaseStringUTFChars(env, fileName, fn);
}

JNIEXPORT void JNICALL
Java_com_league_king_openglsample_native_1renderer_NativeParticleSystemRenderer_step(JNIEnv *env,
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