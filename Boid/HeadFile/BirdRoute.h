#pragma once
#include<glm/glm.hpp>
#include<stack>

class BirdRoute {
public:
	std::stack<glm::vec3>Routes;
	std::stack<float>distance;
	void AddRoute(glm::vec3 _direction,float _distance);
};

