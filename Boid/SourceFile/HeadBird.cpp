#include<HeadBird.h>

HeadBird::HeadBird() {
	rotation = glm::quat(1, 0, 0, 0);
    //velocity = glm::vec3(0.0f);
	position = glm::vec3(0.0f);
    isBeingControl = true;
}

void HeadBird::DealMovement(GLFWwindow * window) {
	float speed = 2.0f;

    glm::vec3 move_direction(0.0f);
    glm::vec3 Front = glm::normalize(velocity);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        move_direction += (Front * speed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glm::length(velocity) > 0.001)
        move_direction -= (Front * speed);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        move_direction.x += speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        move_direction.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        move_direction.y += speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        move_direction.y -= speed;


    if (glm::length(move_direction) > 0) {
        ApplyForce(move_direction);
    }

}

void HeadBird::DealRoute(BirdRoute & route) {
    if (route.distance.empty())return;
    float accSpeed = 2.0f;
    float routeDecrease = glm::length(lastPosition - position);
    lastPosition = position;
    route.distance.top() = route.distance.top() - routeDecrease;
    if (route.distance.top() <= 0.5f) {
        route.distance.pop();
        route.Routes.pop();
        glm::vec3 decreaseForce = velocity * 0.8f;
        ApplyForce(decreaseForce);
        return;
    }
    
    glm::vec3 nextDir = route.Routes.top();
    // 计算叉积
    glm::vec3 cross = glm::cross(nextDir, velocity);

    // 计算叉积的模长平方（避免开方运算）
    float crossLengthSq = glm::dot(cross, cross);
    if (crossLengthSq > 1e-6 || velocity == glm::vec3(0)) {//方向未平稳
        glm::vec3 Force = nextDir * accSpeed;
        
        ApplyForce(Force);
    }


}


void HeadBird::UpdateSelf(float deltaTime) {
    velocity += accleration * deltaTime;

    float speed = glm::length(velocity);
    if (speed > params.head_max_speed) {
        velocity = (velocity / speed) * params.head_max_speed;
    }
    position += velocity * deltaTime;
    accleration = glm::vec3(0.0f);
}