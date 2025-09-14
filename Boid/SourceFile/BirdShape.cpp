#include<BirdShape.h>

BirdShape::BirdShape(){}

#pragma region GeneratePosition


void generate_v_shape(int count, std::vector<glm::vec3>& positions,HeadBird &headBird) {



    glm::vec3 headDir = glm::normalize(headBird.velocity);
    float Rotateangle = acos(glm::dot(glm::vec3(0, 0, 1), headDir)); // 计算旋转角度
    glm::vec3 axis = glm::cross(glm::vec3(0, 0, 1), headDir);  // 计算旋转轴
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotateangle, axis);

    constexpr float angle = glm::radians(45.0f);
    constexpr float spacing = 1.0f;
    positions[0] = glm::vec3(0, 0, 0);
    for (int i = 1; i < count + 1; i++) {
        // 左右分支
        int side = (i % 2 == 0) ? 1 : -1;
        int row = (i + 1) / 2;

        float x = side * row * spacing * cos(angle);
        float z = -row * spacing * sin(angle);
        positions[i] = glm::vec4(x, 0, z,1.0f) * rotation;
    }
}

void generate_sphere(int count, std::vector<glm::vec3>& positions) {
    const float radius = 10.0f;
    const float angle_step = 2.0f * glm::pi<float>() / count;

    positions[0] = glm::vec3(0, 0, 0);
    for (int i = 1; i < count + 1; i++) {
        float xtheta = i * angle_step;
        float ztheta = i * angle_step;
        positions[i] = glm::vec3(
            radius * cos(ztheta) * cos(xtheta),
            radius * cos(ztheta) * sin(xtheta),
            radius * sin(ztheta)
        );
    }
}

void generate_sphere_golden_angle(int count, std::vector<glm::vec3>& positions) {
    const float radius = 8.0f;

    // 使用斐波那契球面分布
    float golden_angle = glm::pi<float>() * (3.0f - sqrt(5.0f));
    positions[0] = glm::vec3(0, 0, 0);
    for (int i = 1; i < count + 1; i++) {
        float y = 1.0f - (i / float(count - 1)) * 2.0f;
        float radius_at_y = sqrt(1.0f - y * y) * radius;

        float theta = golden_angle * i;
        float x = cos(theta) * radius_at_y;
        float z = sin(theta) * radius_at_y;

        positions[i] = glm::vec3(x, y * radius, z);
    }
}


void generate_pyramid(int count, std::vector<glm::vec3>& positions) {
    const float spacing = 2.0f;
    const float width_factor = 0.7f;

    int pyramid_rows = 1;
    int total_birds = 1; // 尖端鸟
    while (total_birds < count + 1) {
        pyramid_rows++;
        total_birds += pyramid_rows * 2 - 1; // 每行增加奇数只鸟
    }


    positions[0] = glm::vec3(0, 0, 0);

    int current_index = 1;
    for (int row = 2; row < pyramid_rows; row++) {
        int birds_in_row = row * 2 - 1; // 当前行鸟的数量（奇数）
        float row_z = -row * spacing; // Z坐标（负值表示向后）

        for (int col = 0; col < birds_in_row; col++) {
            if (current_index >= count + 1) break; // 确保不超过鸟的总数

            // 计算列位置（从 -row 到 row）
            float col_pos = (col - (birds_in_row - 1) / 2.0f);
            float x = col_pos * spacing * width_factor;

            positions[current_index++] = glm::vec3(x, 0, row_z);
        }
    }
}

#pragma endregion

void BirdShape::Init(ShapeType type, int bird_count, std::vector<Bird>& boids_need_shape,HeadBird& _headBird)
{
    shapeType = type;
    birdCount = bird_count;
    relative_Position.resize(bird_count + 1);
    headBird = _headBird;
    Generate_Poisitions();
    ApplyPosition(boids_need_shape);
}

void BirdShape::Set_Type(ShapeType new_type, std::vector<Bird>& boids_need_shape){
	shapeType = new_type;
	Generate_Poisitions();
    ApplyPosition(boids_need_shape);
}

void BirdShape::Generate_Poisitions() {
    switch (shapeType) {
    case V_SHAPE:
        generate_v_shape(birdCount,relative_Position,headBird);
        break;
    case SPHERE:
        generate_sphere(birdCount, relative_Position);
        break;
    case PYRAMID:
        generate_pyramid(birdCount, relative_Position);
        break;
    case SHPERE_GOLDEN_ANGLE:
        generate_sphere_golden_angle(birdCount, relative_Position);
        break;
    case CUSTOM:
        // 自定义形状
        break;
    }
}

void BirdShape::ApplyPosition(std::vector<Bird>& boids_need_shape) {
    for (int i = 1; i < birdCount + 1; i++) {
        boids_need_shape[i - 1].targetPosition = relative_Position[i];
    }
}


