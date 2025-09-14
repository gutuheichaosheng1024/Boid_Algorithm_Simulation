#pragma once

#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp> // GLM 的随机数函数
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include<GL/glew.h>
#include<glfw3.h>
#include<Shader.h>
#include<vector>

//--------------------欧拉角转换为四元数-------------
//// 欧拉角（弧度制）
//float pitch = ...; // 绕 X 轴旋转（俯仰）
//float yaw = ...; // 绕 Y 轴旋转（偏航）
//float roll = ...; // 绕 Z 轴旋转（滚转）
//
//// 直接构造四元数（旋转顺序：Yaw -> Pitch -> Roll）
//glm::quat quaternion = glm::quat(glm::vec3(pitch, yaw, roll));

// BOIDS参数配置
struct BoidParams {
	float shape_force_weight = 4.0f; //形状更改权重
	float min_force_distance = 2.0f; //允许的位置偏差
	float max_force_distance = 6.0f; //需要加速归位的位置
	float perception_radius = 20.0f;      // 感知半径
	float separation_weight = 2.0f;       // 分离权重
	float alignment_weight = 1.0f;        // 对齐权重
	float cohesion_weight = 1.5f;         // 凝聚权重
	float head_separation_weight = 1.0f;       // 头鸟分离权重
	float head_alignment_weight = 1.5f;        // 头鸟对齐权重
	float head_cohesion_weight = 1.0f;         // 头鸟凝聚权重
	float head_cohesion_maxDistance = 20.0f;
	float head_alignment_minDistance = 10.0f;
	float max_forward_distance = 15.0f;
	float max_back_distance = 15.0f;
	float max_speed = 5.0f;               // 最大速度
	float head_max_speed = 5.0f;
	float max_force = 10.0f;               // 最大转向力
	float separation_distance = 3.0f;     // 最小分离距离
	float boundary_sizeX = 200.0f;         // 边界大小X
	float boundary_sizeY = 200.0f;         // 边界大小Y
	float boundary_sizeZ = 200.0f;         // 边界大小Z
	float boundary_force = 8.0f;          // 边界排斥力
};



class Bird {
public:
	static BoidParams params;
	glm::vec3 position;//位置
	glm::quat rotation;//旋转
	glm::vec3 velocity;//速度
	glm::vec3 accleration;//加速度
	glm::vec3 maxSpeed;//最大速度
	glm::vec3 targetPosition;//在形状中的目标位置
	Bird();
	void FromEuler(glm::vec3 angle);
	glm::vec3 ToEuler();
	glm::mat4 ToRotateMartix();
	glm::mat4 ToModelMartix();
	void UpdateSelf(float deltaTime);
	void ApplyForce(glm::vec3& force);
	void BoundaryCheck();
	glm::vec3 GetCloserBoundaryForce();
};
