//
//  Mesh.hpp
//  myOpenGL
//
//  Created by bole on 2019/12/16.
//  Copyright © 2019 吴凡. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include "glm/glm.hpp"

#include <vector>   
#include <string>


struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};


struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures){
        this->vertices = vertices;
//        for (int i = 0; i < vertices.size(); ++i) {
//            std::cout << "position = " << vertices[i].position.x <<";" <<  vertices[i].position.y <<";" << vertices[i].position.z <<std::endl;
//        }
        this->indices = indices;
        this->textures = textures;
        
        setupMesh();
    }
    void draw(Shader &shader);
private:
    unsigned int VAO, VBO, VEO;
    void setupMesh();
};

#endif /* Mesh_hpp */
