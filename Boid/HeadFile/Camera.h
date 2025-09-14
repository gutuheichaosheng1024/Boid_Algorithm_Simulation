#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>
#include "glfw3.h"


class Camera {
public:
	bool first;
	float LastX, LastY;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float pitch, yaw;
	float cameraSpeed, lastFrame, nowFrame;
	bool isQuicked;
	Camera();
	void DealMove(GLFWwindow* window);
	void DealRotate(GLFWwindow* window, float xpos, float ypos);
	glm::mat4 LookAt();
};