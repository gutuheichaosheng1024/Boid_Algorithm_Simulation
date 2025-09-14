#pragma once

#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp> // GLM �����������
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include<GL/glew.h>
#include<glfw3.h>
#include<Shader.h>
#include<vector>

//--------------------ŷ����ת��Ϊ��Ԫ��-------------
//// ŷ���ǣ������ƣ�
//float pitch = ...; // �� X ����ת��������
//float yaw = ...; // �� Y ����ת��ƫ����
//float roll = ...; // �� Z ����ת����ת��
//
//// ֱ�ӹ�����Ԫ������ת˳��Yaw -> Pitch -> Roll��
//glm::quat quaternion = glm::quat(glm::vec3(pitch, yaw, roll));

// BOIDS��������
struct BoidParams {
	float shape_force_weight = 4.0f; //��״����Ȩ��
	float min_force_distance = 2.0f; //�����λ��ƫ��
	float max_force_distance = 6.0f; //��Ҫ���ٹ�λ��λ��
	float perception_radius = 20.0f;      // ��֪�뾶
	float separation_weight = 2.0f;       // ����Ȩ��
	float alignment_weight = 1.0f;        // ����Ȩ��
	float cohesion_weight = 1.5f;         // ����Ȩ��
	float head_separation_weight = 1.0f;       // ͷ�����Ȩ��
	float head_alignment_weight = 1.5f;        // ͷ�����Ȩ��
	float head_cohesion_weight = 1.0f;         // ͷ������Ȩ��
	float head_cohesion_maxDistance = 20.0f;
	float head_alignment_minDistance = 10.0f;
	float max_forward_distance = 15.0f;
	float max_back_distance = 15.0f;
	float max_speed = 5.0f;               // ����ٶ�
	float head_max_speed = 5.0f;
	float max_force = 10.0f;               // ���ת����
	float separation_distance = 3.0f;     // ��С�������
	float boundary_sizeX = 200.0f;         // �߽��СX
	float boundary_sizeY = 200.0f;         // �߽��СY
	float boundary_sizeZ = 200.0f;         // �߽��СZ
	float boundary_force = 8.0f;          // �߽��ų���
};



class Bird {
public:
	static BoidParams params;
	glm::vec3 position;//λ��
	glm::quat rotation;//��ת
	glm::vec3 velocity;//�ٶ�
	glm::vec3 accleration;//���ٶ�
	glm::vec3 maxSpeed;//����ٶ�
	glm::vec3 targetPosition;//����״�е�Ŀ��λ��
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
