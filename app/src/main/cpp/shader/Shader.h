//
// Created by 蔡子仪 on 2019-07-29.
//

#ifndef OPENGL_DEMO_SHADER_H
#define OPENGL_DEMO_SHADER_H

#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <string>

using std::string;

class Shader {
public:
private:
    GLenum type;
    AAssetManager *am;
    string path;
    bool isInit = false;
    GLuint shader;
private:
    char *loadFile();

public:
    GLuint getShader();

    Shader(GLenum t, AAssetManager *a, string p);
};

#endif //OPENGL_DEMO_SHADER_H
