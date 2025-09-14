#include <iostream>
#include<vector>
#include<GL/glew.h>
#include<glfw3.h>

#include<Camera.h>
#include<Shader.h>
#include<Bird.h>
#include<BirdSystem.h>
#include<HeadBird.h>
#include<BirdRoute.h>
#include<BirdShape.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define Bird_Count  100

Camera camera;
std::vector<Bird>birds(Bird_Count);
BirdSystem birdSystem;
glm::mat4 models[Bird_Count];
HeadBird headBird;
glm::mat4 BirdTransform;
BirdRoute route;
BirdShape shape;
void InitShape(BirdShape::ShapeType type) {
    shape.Init(type, Bird_Count, birds,headBird);
}
void InitRoute() {
    route.AddRoute(glm::vec3(0.0, 1.0, 0.0), 5000.0f);
}
void Find() {
    GLenum error;
    while ((error = glGetError()) != 0) {
        std::cout << error << std::endl;
    }
}
void GetModels() {
    for (int i = 0; i < Bird_Count; i++) {
        models[i] = birds[i].ToModelMartix();
    }

}
void Key_Back(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)camera.isQuicked = true;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)camera.isQuicked = false;

    if (headBird.isBeingControl)headBird.DealMovement(window);
    else headBird.DealRoute(route);

    camera.DealMove(window);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.DealRotate(window, xpos, ypos);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)std::cout << "error" << std::endl;


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);








    float vertices[] = {
        // 位置              // 颜色
        // 底面三角形
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 左下
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 右下
         0.0f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 后中

         // 顶点
          0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f, // 顶部

          // 侧面三角形
          // 左面
          -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 左下
           0.0f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 后中
           0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f, // 顶部

           // 右面
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 右下
            0.0f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 后中
            0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f, // 顶部

            // 前面
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 左下
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 右下
             0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f  // 顶部
    };

#pragma region BirdRenderSetUp

    std::string Vertex = "Shader/ShaderV/BaseV.vt";
    std::string Fragment = "Shader/ShaderF/BaseF.fr";
    Shader BirdShader(Vertex, Fragment);

    // 创建VAO和VBO
    unsigned int BirdVao, BirdVbo;
    glGenVertexArrays(1, &BirdVao);
    glGenBuffers(1, &BirdVbo);

    glBindVertexArray(BirdVao);
    glBindBuffer(GL_ARRAY_BUFFER, BirdVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // 颜色属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    GetModels();
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * Bird_Count, &models[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // 假设你的 mat4 数据存储在缓冲区中，起始偏移为 0
    GLuint attributeLocation = 2; // 起始属性位置

    // 为 mat4 的每一列设置属性指针
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(attributeLocation + i);
        glVertexAttribPointer(
            attributeLocation + i, // 属性位置 (2,3,4,5)
            4,                    // 每个属性有4个分量 (vec4)
            GL_FLOAT,             // 数据类型
            GL_FALSE,             // 是否归一化
            sizeof(glm::mat4),    // 整个矩阵的步长
            (void*)(sizeof(glm::vec4) * i) // 当前列的偏移
        );

        // 如果是实例化数据（每个实例一个矩阵）
        glVertexAttribDivisor(attributeLocation + i, 1);
    }
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


#pragma endregion

#pragma region HeadBirdSetUp
    std::string headV = "Shader/ShaderV/HeadV.vt";
    std::string headF = "Shader/ShaderF/HeadF.fr";
    Shader HeadShader(headV, headF);

    GLuint HeadVao, HeadVbo;

    glGenVertexArrays(1, &HeadVao);
    glGenBuffers(1, &HeadVbo);

    glBindVertexArray(HeadVao);
    glBindBuffer(GL_ARRAY_BUFFER, HeadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // 位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // 颜色属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int HeadPositionVbo;
    glGenBuffers(1, &HeadPositionVbo);
    glBindBuffer(GL_ARRAY_BUFFER, HeadPositionVbo);
    BirdTransform = headBird.ToModelMartix();
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &BirdTransform, GL_DYNAMIC_DRAW);
    // 假设你的 mat4 数据存储在缓冲区中，起始偏移为 0
    GLuint attributeLocationHead = 2; // 起始属性位置

    // 为 mat4 的每一列设置属性指针
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(attributeLocationHead + i);
        glVertexAttribPointer(
            attributeLocationHead + i, // 属性位置 (2,3,4,5)
            4,                    // 每个属性有4个分量 (vec4)
            GL_FLOAT,             // 数据类型
            GL_FALSE,             // 是否归一化
            sizeof(glm::mat4),    // 整个矩阵的步长
            (void*)(sizeof(glm::vec4) * i) // 当前列的偏移
        );
        glVertexAttribDivisor(attributeLocation + i, 1);
    }
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#pragma endregion









    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    float nowFrame, lastFrame;
    nowFrame = lastFrame = 0.0f;
    InitRoute();
    InitShape(BirdShape::SHPERE_GOLDEN_ANGLE);
    headBird.isBeingControl = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        BirdShader.Use();


        glm::mat4 view = camera.LookAt();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);


        BirdShader.SetUniformMatrix4fv("view", view);
        BirdShader.SetUniformMatrix4fv("projection", projection);
        nowFrame = glfwGetTime();
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        
        float deltaTime = nowFrame - lastFrame;
        birdSystem.update(deltaTime, birds);
        lastFrame = nowFrame;
        GetModels();
        glBufferSubData(GL_ARRAY_BUFFER,
            0,
            sizeof(glm::mat4)* Bird_Count,
            models);
        glBindVertexArray(BirdVao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 12, Bird_Count); // 12个顶点（4个三角形）



        HeadShader.Use();
        HeadShader.SetUniformMatrix4fv("view", view);
        HeadShader.SetUniformMatrix4fv("projection", projection);

        glBindBuffer(GL_ARRAY_BUFFER, HeadPositionVbo);
        BirdTransform = headBird.ToModelMartix();
        glBufferSubData(GL_ARRAY_BUFFER,
            0,
            sizeof(glm::mat4),
            &BirdTransform);

        glBindVertexArray(HeadVao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 12,1);
        Find();


        Key_Back(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


