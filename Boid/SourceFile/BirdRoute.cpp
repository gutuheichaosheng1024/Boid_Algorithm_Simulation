#include<BirdRoute.h>

void BirdRoute::AddRoute(glm::vec3 _direction, float _distance) {
	Routes.push(_direction);
	distance.push(_distance);
}