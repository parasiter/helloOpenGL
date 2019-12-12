//
//  Camera.hpp
//  myOpenGL
//
//  Created by bole on 2019/12/10.
//  Copyright © 2019 吴凡. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "macro.hpp"

class Camera{
public:
    Camera(float fov,float aspect,float zNear,float zFar);
    // void lookAt(glm::vec3 pos,glm::vec3 lookAt, glm::vec3 up);
    glm::mat4& getProjection(){ return projectionMat;}
    glm::mat4& getObserveMat();
    glm::mat4& getCameraToWorldMat();
    glm::mat4& getTransformMat();

    void setPosition(glm::vec3 pos);
    glm::vec3& getPosition(){return cameraPos;}
    void setRotateX(const float radian);
    void setRotateY(const float radian);

private:
    bool isDirty;
    glm::vec3 cameraPos;
    glm::mat4 projectionMat;
    glm::mat4 observeMat;
    glm::mat4 cameraToWorldMat;
    glm::mat4 transformMat;
    float rotateX, rotateY;
    glm::mat4 tempTranslateMat,tempRotateXMat,tempRotateYMat;
};

#endif /* Camera_hpp */
