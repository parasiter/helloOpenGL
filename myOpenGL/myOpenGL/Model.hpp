//
//  Model.hpp
//  myOpenGL
//
//  Created by bole on 2019/12/16.
//  Copyright © 2019 吴凡. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include "Mesh.hpp"

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model{
public:
    Model(std::string path){
        loadModel(path);
    }
    void draw(Shader &shader);
private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

#endif /* Model_hpp */
