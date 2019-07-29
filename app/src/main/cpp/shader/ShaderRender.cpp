#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-magic-numbers"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
//
// Created by 蔡子仪 on 2019-07-28.
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <jni.h>
#include "time.h"
#include "math.h"
#include <string>


#define LOG_TAG "ShaderRender"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
using namespace ::std;

static GLuint vertexShader;
static GLuint fragmentShader;
static GLuint shaderProgram;
static GLsizei glWidth;
static GLsizei glHeight;
static bool flip = false;
enum TYPE {
    Shader, Linker
};

bool isSuccess(GLuint sp, TYPE type) {
    if (type == Shader) {
        GLint compiled;
        glGetShaderiv(sp, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(sp, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetShaderInfoLog(sp, infoLen, NULL, infoLog);
                LOGE("Error compiling shader:[%s]", infoLog);
                free(infoLog);
            }
            glDeleteShader(sp);
            return false;
        }
        return true;
    } else {
        GLint success;
        glGetProgramiv(sp, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(sp, 512, nullptr, infoLog);
            LOGE("Error compiling linker:[%s]", infoLog);
            return false;
        }
        return true;
    }
}

void initShaderAndProgram(string vertex, string fragment, AAssetManager *am) {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    AAsset *asset = AAssetManager_open(am, vertex.data(), AASSET_MODE_UNKNOWN);
    int len = static_cast<int>(AAsset_getLength(asset));
    char *buffer = new char[len + 1];
    buffer[len] = '\0';
    AAsset_read(asset, buffer, static_cast<size_t>(len));
    AAsset_close(asset);
    glShaderSource(vertexShader, 1, &buffer, nullptr);
    glCompileShader(vertexShader);
    delete[] buffer;
    if (!isSuccess(vertexShader, Shader)) {
        return;
    }

    asset = AAssetManager_open(am, fragment.data(), AASSET_MODE_UNKNOWN);
    len = static_cast<int>(AAsset_getLength(asset));
    buffer = new char[len + 1];
    buffer[len] = '\0';
    AAsset_read(asset, buffer, static_cast<size_t>(len));
    glShaderSource(fragmentShader, 1, &buffer, nullptr);
    glCompileShader(fragmentShader);
    AAsset_close(asset);
    delete[] buffer;
    if (!isSuccess(fragmentShader, Shader)) {
        return;
    }

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!isSuccess(shaderProgram, Linker)) {
        return;
    }

    LOGE("glesInit");
}


extern "C" {
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_load(JNIEnv *env, jobject instance) {
    printf("from load native");
}
}

extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesInit(JNIEnv *env, jobject instance,
                                                                jobject assetManager) {
    if (!(assetManager && env)) {
        LOGE("assetManager is null !");
    }
    AAssetManager *am = AAssetManager_fromJava(env, assetManager);
    initShaderAndProgram("shader/multiAttr/vs.glsl", "shader/multiAttr/fs.glsl", am);
}
extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesResize(JNIEnv *env, jobject instance,
                                                                  jint width, jint height) {
    glWidth = width;
    glHeight = height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesRender(JNIEnv *env, jobject instance) {

    const int POINT_NUMBER = 9;

    GLfloat triangle[POINT_NUMBER] = {
            //第一个三角形
            .5F, .5F, .0F,//右上
            -.5F, .5F, .0F,//左上
            .5F, -.5F, .0F,//右下
    };

    LOGE("begin draw");

    GLuint VAO;
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glUseProgram(shaderProgram);    //必须在设置指针之前


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    glDrawArrays(GL_TRIANGLES, 0, 3);

}



extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesInitUniform(JNIEnv *env,
                                                                       jobject instance,
                                                                       jobject assetManager) {

    if (!(assetManager && env)) {
        LOGE("assetManager is null !");
    }
    AAssetManager *am = AAssetManager_fromJava(env, assetManager);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    AAsset *asset = AAssetManager_open(am, "shader_example/vertex_uniform.glsl",
                                       AASSET_MODE_UNKNOWN);
    int len = static_cast<int>(AAsset_getLength(asset));
    char *buffer = new char[len + 1];
    buffer[len] = '\0';
    AAsset_read(asset, buffer, static_cast<size_t>(len));
    AAsset_close(asset);
    glShaderSource(vertexShader, 1, &buffer, nullptr);
    glCompileShader(vertexShader);
    delete[] buffer;
    if (!isSuccess(vertexShader, Shader)) {
        return;
    }

    asset = AAssetManager_open(am, "shader_example/fragment_uniform.glsl", AASSET_MODE_UNKNOWN);
    len = static_cast<int>(AAsset_getLength(asset));
    buffer = new char[len + 1];
    buffer[len] = '\0';
    AAsset_read(asset, buffer, static_cast<size_t>(len));
    glShaderSource(fragmentShader, 1, &buffer, nullptr);
    glCompileShader(fragmentShader);
    AAsset_close(asset);
    delete[] buffer;
    if (!isSuccess(fragmentShader, Shader)) {
        return;
    }

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!isSuccess(shaderProgram, Linker)) {
        return;
    }

    LOGE("glesInit");
}

extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesRenderUniform(JNIEnv *env,
                                                                         jobject instance) {
    const int POINT_NUMBER = 9;

    GLfloat triangle[POINT_NUMBER] = {
            //第一个三角形
            .5F, .5F, .0F,//右上
            -.5F, .5F, .0F,//左上
            .5F, -.5F, .0F,//右下
    };

    LOGE("begin draw");

    GLuint VAO;
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glUseProgram(shaderProgram);    //必须在设置指针之前
    GLfloat timeValue = clock();
    GLfloat greenValue = sin(timeValue) / 2 + 0.5F;
    GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.f, greenValue, 0.f, 1.f);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STREAM_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);


}extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesInitMultiAttribute(JNIEnv *env,
                                                                              jobject instance,
                                                                              jobject assetManager) {
    if (!(assetManager && env)) {
        LOGE("assetManager is null !");
    }
    AAssetManager *am = AAssetManager_fromJava(env, assetManager);
    initShaderAndProgram("shader/multiAttr/vs.glsl", "shader/multiAttr/fs.glsl", am);

}extern "C"
JNIEXPORT void JNICALL
Java_opengl_panjq_com_opengl_1demo_Render_ShaderRender_glesRenderMultiAttribute(JNIEnv *env,
                                                                                jobject instance) {
    GLfloat vertices[] = {
            // 位置              // 颜色
            0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F,   // 右下
            -0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F,   // 左下
            0.0F, 0.5F, 0.0F, 0.0F, 0.0F, 1.0F    // 顶部
    };
    GLuint VAO;
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    GLint offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    glUseProgram(shaderProgram);
    if (flip) {
        glUniform3f(offsetLocation, 0.1F, 0.1F, 0.1F);
        flip = false;
    } else {
        flip = true;
        glUniform3f(offsetLocation, 0.0F, 0.0F, 0.0F);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}
#pragma clang diagnostic pop