//
// Created by King on 2016/5/24.
//

#include <malloc.h>
#include <android/asset_manager.h>
#include <stdio.h>
#include "BaseRenderer.h"

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

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
                                                    = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                         shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

///
// esFileRead()
//
//    Wrapper for platform specific File open
//
static void *esFileOpen ( void *ioContext, const char *fileName )
{
    void *pFile = NULL;

    if ( ioContext != NULL )
    {
        AAssetManager *assetManager = ( AAssetManager * ) ioContext;
        pFile = AAssetManager_open ( assetManager, fileName, AASSET_MODE_BUFFER );
        if (pFile == NULL)
            LOGE("load file fail");
    } else
    {
        LOGE("ioContext is NULL");
        pFile = fopen ( fileName, "rb" );
    }

    return pFile;
}

///
// esFileRead()
//
//    Wrapper for platform specific File read
//
static int esFileRead ( void * ioContext, void *pFile, int bytesToRead, void *buffer )
{
    int bytesRead = 0;

    if ( pFile == NULL )
    {
        return bytesRead;
    }

    LOGE("bytesToRead is %d", bytesToRead);

    if (ioContext != NULL)
    {
        bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
    }
    else
    {
        bytesRead = fread ( buffer, bytesToRead, 1, pFile );
    }

    return bytesRead;
}

///
// esFileRead()
//
//    Wrapper for platform specific File close
//
static void esFileClose ( void *ioContext, void *pFile )
{
    if ( pFile != NULL )
    {

        if (ioContext != NULL)
        {
            AAsset_close ( pFile );
        }
        else
        {
            fclose ( pFile );
        }
        pFile = NULL;
    }
}

///
// esLoadTGA()
//
//    Loads a 8-bit, 24-bit or 32-bit TGA image from a file
//
char * esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height )
{
    char        *buffer;
    void        *fp;
    TGA_HEADER   Header;
    int          bytesRead;

    // Open the file for reading
    fp = esFileOpen ( ioContext, fileName );

    if ( fp == NULL )
    {
        // Log error as 'error in opening the input file from apk'
        LOGI ( "esLoadTGA FAILED to load : { %s }\n", fileName );
        return NULL;
    }

    bytesRead = esFileRead ( ioContext, fp, sizeof ( TGA_HEADER ), &Header );

    *width = Header.Width;
    *height = Header.Height;

    LOGE("bytesRead is %d", bytesRead);
    LOGE("{IdSize:%d\nMapType:%d\nImageType:%d\nPaletteStart:%d\n"
                 "PaletteSize:%d\nPaletteEntryDepth:%d\nX:%d\n"
                 "Y:%d\nWidth:%d\nHeight:%d\n"
                 "ColorDepth:%d\nDescriptor:%d\n}",
         Header.IdSize, Header.MapType, Header.ImageType, Header.PaletteStart, Header.PaletteSize,
         Header.PaletteEntryDepth, Header.X, Header.Y, Header.Width, Header.Height,
         Header.ColorDepth, Header.Descriptor);

    if ( Header.ColorDepth == 8 ||
         Header.ColorDepth == 24 || Header.ColorDepth == 32 )
    {
        int bytesToRead = sizeof ( char ) * ( *width ) * ( *height ) * Header.ColorDepth / 8;

        LOGE("bytesToRead is %d", bytesToRead);

        // Allocate the image data buffer
        buffer = ( char * ) malloc ( bytesToRead );

        if ( buffer )
        {
            bytesRead = esFileRead ( ioContext, fp, bytesToRead, buffer );
            esFileClose ( ioContext, fp );

            LOGI ( "esLoadTGA success to load : { %s }\n", fileName );

            return ( buffer );
        }
    }

    LOGE ( "esLoadTGA fail to load : { %s }\n", fileName );
    return ( NULL );
}