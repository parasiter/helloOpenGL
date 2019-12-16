#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <math.h>
#include "stb_image.h"
#include "Shader.hpp"
#include "macro.hpp"
#include "Camera.hpp"
#include "Model.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,Camera &camera,float time);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VAO,VBO, VEO;


struct Material{
    glm::vec3 ambientColor;
    glm::vec3 diffColor;
    glm::vec3 specColor;
    float shininess;
    Material(glm::vec3 ambient,glm::vec3 diff,glm::vec3 spec,float specF){
        ambientColor = ambient;
        diffColor = diff;
        specColor = spec;
        shininess = specF;
    }
};
struct LightFactor{
    glm::vec3 ambientFactor;
    glm::vec3 diffFactor;
    glm::vec3 specFactor;
    LightFactor(glm::vec3 ambient,glm::vec3 diff,glm::vec3 spec){
        ambientFactor = ambient;
        diffFactor = diff;
        specFactor = spec;
    }
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    Camera camera(glm::radians(45.0f),800.0f/450.0f, 0.1f, 100.0f);
    camera.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    // 编译链接shader
    Shader shaderProgram("shaders/sample.vs","shaders/sample.fs");
    shaderProgram.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0,0.0,-10.0f));
    shaderProgram.setMat4("projection", projection);
    shaderProgram.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
    shaderProgram.setMat4("model", model);
    
    Model _model("/Users/bole/client/helloOpenGL/myOpenGL/myOpenGL/resource/nanosuit.obj");

    // shaderProgram.setFloat("material.shininess",32.0f);
        //创建聚光灯
    // shaderProgram.setVec3("spotLight.lightPos",glm::vec3(1.5f,  0.1f,  -1.0f));
    // shaderProgram.setVec3("spotLight.lightDir",glm::vec3(-1.5f,  -0.1f,  -1.0f));
    // shaderProgram.setFloat("spotLight.innerCutOff",glm::cos(glm::radians(12.5f)));
    // shaderProgram.setFloat("spotLight.outerCutOff",glm::cos(glm::radians(17.5f)));

    //     //设置光照属性
    // LightFactor lightFactor(glm::vec3(0.3f,0.3f,0.3f),glm::vec3(0.9f,0.9f,0.9f),glm::vec3(1.,1.0,1.0f));
    // shaderProgram.setVec3("lightFactor.ambientFactor",lightFactor.ambientFactor);
    // shaderProgram.setVec3("lightFactor.diffFactor",lightFactor.diffFactor);
    // shaderProgram.setVec3("lightFactor.specFactor",lightFactor.specFactor);

    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    // model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    // shaderProgram.setMat4("transform", camera.getTransformMat());
    // shaderProgram.setMat4("model", model);
    // shaderProgram.setMat4("normalRotate", glm::mat4(1.0f));
    // shaderProgram.setVec3("observePos",camera.getPosition());
    
//    Shader lightShader("shaders/light.vs","shaders/light.fs");
    

    //开启线框模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //统一配置
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    //创建摄像机
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window,camera,deltaTime);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        shaderProgram.use();
        _model.draw(shaderProgram);
        //绘制物体
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Camera& camera, float time)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + glm::vec3(-2.0,0.0,0.0)*time);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + glm::vec3(2.0,0.0,0.0)*time);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + glm::vec3(0.0,0.0,-2.0f)*time);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + glm::vec3(0.0,0.0,2.0f)*time);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
