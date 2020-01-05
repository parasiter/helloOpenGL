//
//  EasyDraw.hpp
//  myOpenGL
//
//  Created by bole on 2020/1/2.
//  Copyright © 2020 吴凡. All rights reserved.
//

#ifndef EasyDraw_hpp
#define EasyDraw_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"

unsigned int loadTexture(std::string path);
unsigned int loadCubeTextures(std::vector<std::string> faces);

class Object {
public: 
    Object();
    virtual void setShader(Shader shader);
    virtual void setTexture(unsigned int id) = 0;
    virtual void draw(Shader shader) = 0;
    virtual void draw() = 0;
    virtual void init() = 0;
    Shader shader;
    unsigned int textureID;
    unsigned int VAO;
};
//四边形
class Quad:public Object {
public:
    Quad(){
        init();
    }
    void loadTextures(std::string path);
    virtual void setTexture(unsigned int id);
    virtual void draw(Shader shader);
    virtual void draw();
    virtual void init();
};

class Cube:public Object{
public:
    Cube(){
        init();
    }
    void loadTextures(std::string path);
    virtual void setTexture(unsigned int id);
    virtual void draw(Shader shader);
    virtual void draw();
    virtual void init();
};

class SkyBox:public Object{
public:
    SkyBox(){
        init();
    }
    void loadTextures(std::vector<std::string> faces);
    virtual void setTexture(unsigned int id);
    virtual void draw(Shader shader);
    virtual void draw();
    virtual void init();
};








#endif /* EasyDraw_hpp */
