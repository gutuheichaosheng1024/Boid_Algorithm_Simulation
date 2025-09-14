#pragma once
#include<vector>
#include<Bird.h>
#include<HeadBird.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BirdShape {
public:
	enum ShapeType
	{
		V_SHAPE,
		CIRCLE,
		PYRAMID,
		SPHERE,
		SHPERE_GOLDEN_ANGLE,
		CUSTOM
	};

	int birdCount;
	ShapeType shapeType;
	std::vector<glm::vec3>relative_Position;
	HeadBird headBird;

	BirdShape();
	void Init(ShapeType type, int bird_count, std::vector<Bird>& boids_need_shape, HeadBird& _headBird);
	void Set_Type(ShapeType new_type, std::vector<Bird>& boids_need_shape);
	void Generate_Poisitions();
	void ApplyPosition(std::vector<Bird>& boids_need_shape);

};
