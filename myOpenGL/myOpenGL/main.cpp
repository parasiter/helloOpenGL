#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include "macro.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include <vector>
#include <random>
#include "EasyDraw.hpp"


#define drawNum 2000
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,Camera &camera,float time);
bool createFBO();

void renderScene(Object& , Shader &shader);
void renderPlane();
void renderCube();
void renderQuad();

void initRock(Model &model,const unsigned int num);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VAO,VBO, VEO;
unsigned int FBO, textureBuffer;
unsigned int skyVAO, skyVBO;
unsigned int floorTexID,wallTexID;
unsigned int quadVAO = 0;
unsigned int quadVBO;

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
    
    Camera camera(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 150.0f);
    camera.setPosition(glm::vec3(0.0f, 30.0f, 45.0f));
//    camera.setRotateX(glm::radians(-35.0f));
    glm::mat4 projMat = camera.getProjectionMat();
    glm::mat4 view = camera.getViewMat();
//    glm::mat4 normMat = glm::mat4(glm::mat3(view));
    
    Cube cube;
    cube.loadTextures("floor.jpg");
    
    //深度shader
    createFBO();
    Shader depthShader("shaders/depth.vs","shaders/depth.gs","shaders/depth.fs");
    depthShader.use();
    glm::vec3 lightPos = glm::vec3(0.,30.,0.);
    glm::mat4 transMat[6];
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.f, 0.1f, 55.f);
    
    transMat[0] = projection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0,0.,0.), glm::vec3(0.,-1.,0.));
    transMat[1] = projection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,0.,0.), glm::vec3(0.,-1.,0.));
    transMat[2] = projection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,1.,0.), glm::vec3(0.,0.,1.));
    transMat[3] = projection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,-1.,0.), glm::vec3(0.,0.,-1.));
    transMat[4] = projection * glm::lookAt(lightPos, lightPos + glm::vec3(0.,0.,1.), glm::vec3(0.,-1.,0.));
    transMat[5] = projection * glm::lookAt(lightPos, lightPos + glm::vec3(0.,0.,-1.), glm::vec3(0.,-1.,0.));
    depthShader.setVec3("lightPos", lightPos);
    depthShader.setFloat("far_plane", 100.f);
//    depthShader.setMat4("transMat",transMat[0]);
    for (int i = 0  ; i<6 ; ++i) {
        depthShader.setMat4(("transMat[" + std::to_string(i) + "]").c_str(),transMat[i]);
    }
    
    
    //测试shader
    SkyBox skyBox;
    skyBox.setTexture(textureBuffer);
    Shader skyBoxShader("shaders/skyBox.vs","shaders/skyBox.fs");
    skyBoxShader.use();
    skyBoxShader.setMat4("projection",projMat);
    skyBoxShader.setMat4("view",view);
    skyBoxShader.setMat4("model",glm::scale(glm::mat4(1.0), glm::vec3(30.,30.,30.)));
    skyBoxShader.setVec3("lightPos", lightPos);
    skyBoxShader.setInt("textureID",0);
    
    skyBox.setShader(skyBoxShader);
    
    Shader normal("shaders/normal.vs","shaders/normal.fs");
    normal.use();
    normal.setMat4("projection",projMat);
    normal.setMat4("view",view);
//    normal.setMat4("normMat",normMat);
    normal.setInt("textureID",0);
    normal.setInt("textureCubeID",1);
    normal.setVec3("lightPos",lightPos);
    normal.setFloat("far_plane", 100.f);
    
    
    //开启线框模式
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //统一配置
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
        cube.shader.setMat4("view", camera.getViewMat());
         glBindFramebuffer(GL_FRAMEBUFFER,FBO);
        glViewport(0,0,1024,1024);
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LESS);
        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        renderScene(cube,depthShader);
        
        
        cube.shader.setMat4("view", camera.getViewMat());
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,800*2,600*2);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //使用天空盒测试
//        skyBox.shader.use();
//        skyBox.shader.setMat4("view", camera.getViewMat());
//        skyBox.draw();

         renderScene(cube,normal);

        // glBindFramebuffer(GL_FRAMEBUFFER,0);
        // glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        // normalShader.use();
        // normalShader.setMat4("view",camera.getViewMat());
        
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D,textureBuffer);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // glActiveTexture(GL_TEXTURE0);
        // renderScene(normalShader);
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
        camera.setRotateY(camera.getRotateY() + 1.0f*time);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.setRotateY(camera.getRotateY() - 1.0f*time);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.setRotateX(camera.getRotateX() + 1.0f*time);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.setRotateX(camera.getRotateX() - 1.0f*time);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
unsigned int count = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    std::cout << "resize times = "<< ++count << std::endl;
    glViewport(0, 0, width, height);
}

bool createFBO(){
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    
    glGenTextures(1,&textureBuffer);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textureBuffer);
    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_DEPTH_COMPONENT,1024,1024,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, textureBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,textureBuffer,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    return true;
    
}

void initRock(Model &model,const unsigned int num){
    float radius = 20.f;
    float radiusOffset = 3.f;
    std::default_random_engine e;
    std::uniform_real_distribution<float> u(0, 1);
    float theta = 0;
    glm::mat4 trans[num];
    for (int i = 0; i < num; ++i) {
        glm::mat4 temp(1.0f);
        theta += 360.0/num;
//        float offset = u(e) * 2 * radiusOffset - radiusOffset;
        float x = radius * sin(theta);
        float z = radius * cos(theta);
//        std::cout << "(" << x << "," << y << ")" <<std::endl;
        
        temp = glm::translate(temp,glm::vec3(x + (u(e) -1.) * radiusOffset * 1.5 ,((u(e) -1.) * radiusOffset + 2.5) * 1,z + (u(e) -1.) * radiusOffset * 1.5));
        temp = glm::scale(temp,glm::vec3(0.1,0.1,0.1));
        temp = glm::rotate(temp, glm::radians(u(e)*360),glm::vec3(1.,1.,1.));
        trans[i] = temp;
        
    }
    
    unsigned int vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,num*sizeof(glm::mat4),&trans[0],GL_STATIC_DRAW);
    for(unsigned int i = 0; i < model.meshes.size(); ++ i){
        GLsizei vec4Size = sizeof(glm::vec4);
        glBindVertexArray(model.meshes[i].VAO);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void*)0);
        glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void*) vec4Size);
        glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void*) (2 * vec4Size));
        glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void*) (3 * vec4Size));
        
        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);
        
        glBindVertexArray(0);
    }
}

void renderScene(Object& cube, Shader &shader)
{
    cube.setShader(shader);
    cube.shader.use();
    for (int i = 0; i< 6; ++ i) {
        cube.shader.use();
        glm::mat4 model = glm::mat4(1.0);
        if (i == 0) {
            model = glm::scale(model, glm::vec3(50.,50.,50.));
        }else if(i == 1){
            model = glm::translate(glm::mat4(1.),glm::vec3(30. , 30. , -40.));
            model = glm::scale(model, glm::vec3(4.,4.,4.));
            model = glm::rotate(model, (float)glm::radians(40.), glm::vec3(1.,0.,0.));
        }else if(i == 2){
            model = glm::translate(glm::mat4(1.),glm::vec3(-10.,-30., -35.));
            model = glm::scale(model, glm::vec3(5,5,5));
            model = glm::rotate(model, (float)glm::radians(40.), glm::vec3(0.,1.,1.));
        }else if(i == 3){
            model = glm::translate(glm::mat4(1.),glm::vec3(15.,-20., -30.));
            model = glm::scale(model, glm::vec3(6.,6.,6.));
        }else if(i == 4){
            model = glm::translate(glm::mat4(1.),glm::vec3(35.,15., -10.));
            model = glm::scale(model, glm::vec3(3,3,3));
            model = glm::rotate(model, (float)glm::radians(20.), glm::vec3(1.,0.,0.));
        }else if(i == 5){
            model = glm::translate(glm::mat4(1.),glm::vec3(-20.,-20, 0.));
            model = glm::scale(model, glm::vec3(4.,4.,4.));
        }
        glm::mat4 normalMat = glm::mat4(glm::mat3(model));
        if (i == 0) {
            normalMat = - normalMat;
        }
        cube.shader.use();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP,textureBuffer);
        cube.shader.setMat4("model", model);
        cube.shader.setMat4("normMat",normalMat);
        cube.draw();
    }
}

void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
