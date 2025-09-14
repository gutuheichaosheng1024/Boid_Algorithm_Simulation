#include<Bird.h>
#include<cmath>
#include<iostream>

BoidParams Bird::params;


// 初始化随机数引擎（只需初始化一次）
std::default_random_engine& getRandomEngine() {
    static std::default_random_engine engine(std::random_device{}());
    return engine;
}


glm::quat randomQuat() {
    std::normal_distribution<float> normalDist(0.0f, 1.0f);
    // 生成4个独立的正态分布随机数
    float w = normalDist(getRandomEngine());
    float x = normalDist(getRandomEngine());
    float y = normalDist(getRandomEngine());
    float z = normalDist(getRandomEngine());

    // 归一化为单位四元数
    return glm::normalize(glm::quat(w, x, y, z));
}

glm::vec3 randomVec3(float range) {
    std::uniform_real_distribution<float> velocityDist(-range, range);
    glm::vec3 position = glm::vec3(
        velocityDist(getRandomEngine()),
        velocityDist(getRandomEngine()),
        velocityDist(getRandomEngine())
    );//初始速度随机化
    return position;
}




Bird::Bird() {
    position = randomVec3(30.0f);
    velocity = randomVec3(0.5f);
    rotation = randomQuat();
    accleration = glm::vec3(0.1f);
}

glm::quat rotation_quaternion_between_vectors(glm::vec3 a, glm::vec3 b) {
    a = glm::normalize(a);
    b = glm::normalize(b);

    // 计算旋转轴和角度
    glm::vec3 axis = glm::cross(a, b);
    float cos_angle = glm::dot(a, b);

    // 处理特殊情况
    if (glm::length(axis) < 0.0001f) {
        if (cos_angle < 0) {
            // 180度旋转，需要垂直轴
            glm::vec3 tmp = glm::abs(a.x) > 0.9f ? glm::vec3(0.0f, 1.0f, 0.0f)
                : glm::vec3(1.0f, 0.0f, 0.0f);
            axis = glm::normalize(glm::cross(a, tmp));
            return glm::angleAxis(glm::pi<float>(), axis);
        }
        // 相同方向，返回单位四元数
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    axis = glm::normalize(axis);
    float angle = glm::acos(glm::clamp(cos_angle, -1.0f, 1.0f));

    return glm::angleAxis(angle, axis);
}



void Bird::FromEuler(glm::vec3 angle) {
    float pitch = glm::radians(angle.x);  // 俯仰角
    float yaw = glm::radians(angle.y);    // 偏航角
    float roll = glm::radians(angle.z);   // 滚转角
    rotation = glm::quat(glm::vec3(pitch, yaw, roll));
}

glm::vec3 Bird::ToEuler() {
    glm::vec3 eulerRadians = glm::eulerAngles(rotation);

    // 将弧度转换为角度
    glm::vec3 eulerDegrees = glm::degrees(eulerRadians);
    return eulerDegrees;
}

glm::mat4 Bird::ToRotateMartix() {
    glm::mat4 rotationMatrix = glm::mat4_cast(rotation_quaternion_between_vectors(glm::vec3(0.0f, 1.0f, 0.0f), velocity));
    return rotationMatrix;
}

glm::mat4 Bird::ToModelMartix() {
    glm::mat4 model = ToRotateMartix();
    model[3] = glm::vec4(position, 1.0f);
    return model;
}

void Bird::UpdateSelf(float deltaTime) {
    velocity += accleration * deltaTime;

    float speed = glm::length(velocity);
    if (speed > params.max_speed) {
        velocity = (velocity / speed) * params.max_speed;
    }
    position += velocity * deltaTime;
    accleration = glm::vec3(0.0f);

}

// 应用力（限制最大力）
void Bird::ApplyForce(glm::vec3& force) {
    accleration += glm::length(force) > params.max_force ?
        glm::normalize(force) * params.max_force : force;
}


glm::vec3 Bird::GetCloserBoundaryForce(){
    glm::vec3 Force(0.0f);
    Force.x = -pow(position.x / params.boundary_sizeX, 3);
    Force.y = -pow(position.y / params.boundary_sizeY, 3);  
    Force.z = -pow(position.z / params.boundary_sizeZ, 3);
    Force *= params.boundary_force;

    return Force;
}

void Bird::BoundaryCheck() {
    glm::vec3 DiffForce = GetCloserBoundaryForce();
    ApplyForce(DiffForce);
}

