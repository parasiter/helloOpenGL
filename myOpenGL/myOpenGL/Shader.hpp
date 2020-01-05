//
//  Shader.hpp
//  myOpenGL
//
//  Created by bole on 2019/12/9.
//  Copyright © 2019 吴凡. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "macro.hpp"

#include "macro.hpp"

enum ShaderType{
    VERT_SHADER,
    FRAG_SHADER,
    GEO_SHADER
};

int complieShader(const char* shaderContent, int shaderType);

class Shader{
public:
    Shader();
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(const GLchar* vertexPath, const GLchar* geometry, const GLchar* fragmentPath);
    std::string getCode(const GLchar* path);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 mat) const;
    void setVec3(const std::string &name, glm::vec3 vec) const;
    unsigned int ID;
};

#endif /* Shader_hpp */
