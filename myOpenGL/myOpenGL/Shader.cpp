//
//  Shader.cpp
//  myOpenGL
//
//  Created by bole on 2019/12/9.
//  Copyright © 2019 吴凡. All rights reserved.
//

#include "Shader.hpp"

unsigned int compileShader(const char* shaderContent, int shaderType)
{
    int shader;
    if (shaderType == VERT_SHADER )
        shader = glad_glCreateShader(GL_VERTEX_SHADER);
    else if(shaderType == FRAG_SHADER)
        shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
    else if(shaderType == GEO_SHADER)
        shader = glad_glCreateShader(GL_GEOMETRY_SHADER);
    else{
        std::cout << "ERROR::SHADER::::INVALID_SHADER_TYPE\n" << std::endl;
        return 0;
    }
    glShaderSource(shader, 1, &shaderContent, NULL);
    glad_glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

Shader::Shader(){
    ID = 0;
}

Shader::Shader(const GLchar* vertPath, const GLchar* fragPath){
    std::string vertCode = getCode(vertPath);
    std::string fragCode = getCode(fragPath);
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();
    
    unsigned int vShader = compileShader(vShaderCode, VERT_SHADER);
    unsigned int fShader = compileShader(fShaderCode, FRAG_SHADER);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vShader);
    glAttachShader(shaderProgram,fShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    ID = shaderProgram;
    
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

Shader::Shader(const GLchar* vertexPath, const GLchar* geoPath, const GLchar* fragmentPath){
    std::string vertCode = getCode(vertexPath);
    std::string fragCode = getCode(fragmentPath);
    std::string geoCode = getCode(geoPath);
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();
    const char* gShaderCode = geoCode.c_str();
    
    unsigned int vShader = compileShader(vShaderCode, VERT_SHADER);
    unsigned int fShader = compileShader(fShaderCode, FRAG_SHADER);
    unsigned int gShader = compileShader(gShaderCode, GEO_SHADER);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vShader);
    glAttachShader(shaderProgram,fShader);
    glAttachShader(shaderProgram,gShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    glDeleteShader(gShader);
    ID = shaderProgram;
    
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

std::string Shader::getCode(const GLchar* path){
    std::string code;
    std::ifstream shaderFile;
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try{
        std::string _path = path;
        _path = ROOT_PATH + _path;
        std::cout << "path::" << _path <<  std::endl;
        std::stringstream shaderStream;
        shaderFile.open(_path.c_str());
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
        return code;
    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return "";
    }
}




void Shader::use(){
    glUseProgram(ID);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int) value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setMat4(const std::string &name, glm::mat4 trans) const {
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,glm::value_ptr(trans));
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const{
    glUniform3fv(glGetUniformLocation(ID,name.c_str()),1,glm::value_ptr(vec));
}
