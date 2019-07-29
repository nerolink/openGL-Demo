//
// Created by 蔡子仪 on 2019-07-29.
//

#include "Shader.h"
#include <utility>

Shader::Shader(GLenum t, AAssetManager *a, string p) : type(t), path(std::move(p)), am(a) {

}

GLuint Shader::getShader() {
    if (isInit) {
        return shader;
    }
    shader = glCreateShader(type);
    char *source = loadFile();
    int len = strlen(source);
    glShaderSource(shader, 1, &source, &len);
    glCompileShader(shader);
    return shader;
}

char *Shader::loadFile() {
    AAsset *as = AAssetManager_open(am, path.data(), AASSET_MODE_UNKNOWN);
    int len = static_cast<int>(AAsset_getLength(as));
    char *buffer = new char[len + 1];
    buffer[len] = '\0';
    return buffer;
}
