//
//  Mesh.cpp
//  myOpenGL
//
//  Created by bole on 2019/12/16.
//  Copyright © 2019 吴凡. All rights reserved.
//

#include "Mesh.hpp"

void Mesh::setupMesh(){
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VEO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,texCoords));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader){
    shader.use();
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    for (unsigned int i = 0; i < textures.size() ; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        Texture tex = textures[i];
        glBindTexture(GL_TEXTURE_2D,tex.id);
        if (tex.type == "texture_diffuse") {
            shader.setInt("material." + tex.type + std::to_string(diffuseNum++), i);
        }else if(tex.type == "texture_specular"){
            shader.setInt("material." + tex.type + std::to_string(specularNum++), i);
        }

    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
