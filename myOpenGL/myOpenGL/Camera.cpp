//
//  Camera.cpp
//  myOpenGL
//
//  Created by bole on 2019/12/10.
//  Copyright © 2019 吴凡. All rights reserved.
//

#include "Camera.hpp"

Camera::Camera(float fov,float aspect,float zNear,float zFar){
    projectionMat = glm::perspective(fov, aspect, zNear, zFar);
    isDirty = false;
    observeMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, -1.0f) , glm::vec3(0.0f, 1.0f, 0.0f));
    cameraToWorldMat = glm::mat4(1.0f);
    transformMat = projectionMat*observeMat*cameraToWorldMat;
    rotateX = 0.0f;
    rotateY = 0.0f;
    tempTranslateMat = glm::mat4(1.0f);
    tempRotateXMat = glm::mat4(1.0f);
    tempRotateYMat = glm::mat4(1.0f);
}

// void Camera::lookAt(glm::vec3 pos,glm::vec3 lookAt, glm::vec3 up){
//     observeMat = glm::lookAt(pos,lookAt,up);
//     cameraPos = pos;
// }

//必须先执行位移再执行旋转
void Camera::setPosition(glm::vec3 pos){
    cameraPos = pos;
    tempTranslateMat = glm::translate(glm::mat4(1.0f),-cameraPos);
    isDirty = true;
}

void Camera::setRotateX(const float radian){
    rotateX = radian;
    tempRotateXMat = glm::rotate(glm::mat4(1.0f),-rotateX,glm::vec3(1.0,0.0,0.0));
    isDirty = true;
}

float Camera::getRotateX(){
    return rotateX;
}

void Camera::setRotateY(const float radian){
    rotateY = radian;
    tempRotateYMat = glm::rotate(glm::mat4(1.0f),-rotateY,glm::vec3(0.0,1.0,0.0));
    isDirty = true;
}

float Camera::getRotateY(){
    return rotateY;
}

glm::mat4& Camera::getObserveMat(){
    return observeMat;
}

glm::mat4& Camera::getTransformMat(){
    transformMat = projectionMat*observeMat*getCameraToWorldMat();
    return transformMat;
}

glm::mat4& Camera::getCameraToWorldMat(){
    if(isDirty){
        cameraToWorldMat = tempRotateXMat*tempRotateYMat*tempTranslateMat;
        isDirty = false;
    }
    return cameraToWorldMat;
}

glm::mat4& Camera::getViewMat(){
    viewMat = observeMat * getCameraToWorldMat();
    return viewMat;
}

