#pragma once
#include<Bird.h>
#include<BirdRoute.h>
#include<stack>
#include<glfw3.h>

class HeadBird :public Bird {
public:
	bool isBeingControl;
	glm::vec3 lastPosition;
	float nowFrame = 0.0f;
	float lastFrame = 0.0f;
	void DealMovement(GLFWwindow * window);
	void DealRoute(BirdRoute& route);
	void UpdateSelf(float deltaTime);
	HeadBird();
};