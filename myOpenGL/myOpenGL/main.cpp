#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <math.h>
#include "stb_image.h"
#include "Shader.hpp"
#include "macro.hpp"
#include "Camera.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,Camera &camera,float time);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VAO,VBO, VEO;
void initVertexData ()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0,1,2,
        1,2,3
    };
    
    glad_glGenBuffers(1,&VBO);
    glad_glGenVertexArrays(1,&VAO);
    glad_glBindVertexArray(VAO);
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
//    glad_glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3 *sizeof(float)));
    glad_glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    
    glad_glEnableVertexAttribArray(0);
    glad_glEnableVertexAttribArray(1);
//    glad_glEnableVertexAttribArray(2);
    
    glad_glGenBuffers(1,&VEO);
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VEO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
}

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
    // 编译链接shader
    Shader shaderProgram("shaders/sample.vs","shaders/sample.fs");
    shaderProgram.use();
    shaderProgram.setInt("texture1",0);
    shaderProgram.setInt("texture2",1);
    
    //初始化顶点数据
    initVertexData();
    
    //初始化纹理
    ////
    unsigned int texture1,texture2;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    int width,height,nrChannels;
    char path[128] = ROOT_PATH;
    unsigned char *data = stbi_load(strcat(path,"resource/container.jpg"), &width, &height, &nrChannels, 0);
    std::cout<< "width: "<< width << "height: "<< height << "nrChannels: "<< nrChannels << std::endl;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    strcpy(path,ROOT_PATH);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(strcat(path,"resource/awesomeface.png"), &width, &height, &nrChannels, 0);
    std::cout<< "width: "<< width << "height: "<< height << "nrChannels: "<< nrChannels << std::endl;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);
    //结束
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    //开启线框模式
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //统一配置
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    Camera camera(glm::radians(45.0f),800.0f/450.0f, 0.1f, 100.0f);
    camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        
//        camera.setRotateX(glfwGetTime());
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window,camera,deltaTime);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        
        shaderProgram.use();
        
        //测试矩阵
//        for (int i = 0; i < 10; ++i) {
//            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/450.0f, 0.1f, 100.0f);
//
////            float radius = 20.0f;
////            float camX = radius * sin(glfwGetTime());
////            float camZ = radius * cos(glfwGetTime());
//
//            glm::mat4 lookAt = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, -3.0f) , glm::vec3(0.0f, 1.0f, 0.0f));
//
//            glm::mat4 trans = glm::mat4(1.0);
//            trans = glm::translate(trans,cubePositions[i]);
//
//            //因为是列主序，乘法顺序为 Mt * Mr * vec4,因此模型矩阵为Mtranslate * Mrotate
//            glm::mat4 model = glm::rotate(trans,(float)glfwGetTime() , glm::vec3(1.0,1.0,0.0));
//
//            glm::mat4 transform = projection*lookAt;
//            shaderProgram.setMat4("transform", transform);
////            shaderProgram.setMat4("lookAt", lookAt);
//            shaderProgram.setMat4("model", model);
//
//            glad_glBindVertexArray(VAO);
//            glDrawArrays(GL_TRIANGLES,0,36);
//        }
        
        for (int i = 0; i < 10; ++i) {
//            camera.setRotateX(glm::radians());

            glm::mat4 trans = glm::mat4(1.0);
            trans = glm::translate(trans,cubePositions[i]);
            glm::mat4 model = glm::rotate(trans,(float)glfwGetTime() , glm::vec3(1.0,1.0,0.0));

            shaderProgram.setMat4("transform", camera.getTransformMat());
            shaderProgram.setMat4("model", model);

            glad_glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES,0,36);
        }
//
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
