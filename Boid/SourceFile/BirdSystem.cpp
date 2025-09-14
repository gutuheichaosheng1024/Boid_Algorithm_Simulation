#include<BirdSystem.h>
#include<iostream>


float angleBetweenVectors(const glm::vec3& a, const glm::vec3& b) {
    // 1. ������
    float dot = glm::dot(a, b);

    // 2. ������������
    float lenA = glm::length(a);
    float lenB = glm::length(b);

    // 3. ���������
    if (lenA < 1e-6f || lenB < 1e-6f) {
        return 0.0f; // ���������н�δ���壬����0
    }

    // 4. ����нǵ�����ֵ
    float cosTheta = dot / (lenA * lenB);

    // 5. Լ����Χ����ֹ������
    cosTheta = glm::clamp(cosTheta, -1.0f, 1.0f);

    // 6. ����нǣ����ȣ�
    return glm::acos(cosTheta);
}

void applyThreeRules(std::vector<Bird>& boids) {
    for (auto& boid : boids) {
        glm::vec3 separation(0.0f);
        glm::vec3 alignment(0.0f);
        glm::vec3 cohesion(0.0f);
        int neighbors_count = 0;

        // Ѱ���ھӲ�������Ϊ����
        for (const auto& other : boids) {
            if (&boid == &other) continue;

            glm::vec3 diff = boid.position - other.position;
            float distance = glm::length(diff);

            // ����Ƿ��ڸ�֪��Χ��
            if (distance < Bird::params.perception_radius) {
                neighbors_count++;

                // ���룺�������ھ���ײ
                if (distance < Bird::params.separation_distance) {
                    separation += diff / (distance * distance);
                }

                // ���룺ƥ���ھӵ�ƽ������
                alignment += other.velocity;

                // ���ۣ����ھӵ�ƽ��λ���ƶ�
                cohesion += other.position;
            }
        }

        glm::vec3 totalForce = glm::vec3(0.0f);
        // Ӧ����Ϊ����
        if (neighbors_count > 0) {
            // �������
            if (glm::length(separation) > 0) {
                separation = glm::normalize(separation) * Bird::params.separation_weight;
                totalForce += separation;
            }

            // �������
            alignment /= static_cast<float>(neighbors_count);
            if (glm::length(alignment) > 0) {
                alignment = glm::normalize(alignment) * Bird::params.alignment_weight;// - boid.velocity
                totalForce += alignment;
            }

            // ���۹���
            cohesion /= static_cast<float>(neighbors_count);
            glm::vec3 desired = cohesion - boid.position;
            if (glm::length(desired) > 0) {
                desired = glm::normalize(desired) * Bird::params.max_speed;
                glm::vec3 steer = desired - boid.velocity;
                steer = glm::normalize(steer) * Bird::params.cohesion_weight;
                totalForce += steer;
            }
            boid.ApplyForce(totalForce);
        }
    }
}

void applyHeadBirdForceWithNoShape(float delta_time, std::vector<Bird>& boids, HeadBird& headBird) {
    glm::vec3 head_dir = glm::normalize(headBird.velocity);
    float head_speed = glm::length(headBird.velocity);
    if (head_speed == 0.0f)return;
    for (auto& boid : boids) {
        glm::vec3 to_head = headBird.position - boid.position;
        float distance = glm::length(to_head);//��ͷ��ľ���
        glm::vec3 cohesion = glm::normalize(to_head);//��������
        glm::vec3 separation = -cohesion;//��������
        glm::vec3 alignment = head_dir;//��������

        float dymatic_separation_param = Bird::params.head_separation_weight;
        float dymatic_cohension_param = Bird::params.head_cohesion_weight;
        float dymatic_alignment_param = Bird::params.head_alignment_weight;
        if (distance > Bird::params.head_cohesion_maxDistance) {
            dymatic_cohension_param *= 2.0f;
        }
        if (distance < Bird::params.head_alignment_minDistance) {
            dymatic_alignment_param *= 2.0f;
        }

        glm::vec3 totalForce(0.0f);


        float projection = glm::dot(to_head, head_dir);
        glm::vec3 boid_dir = glm::normalize(boid.velocity);
        float boid_speed = glm::length(boid.velocity);
        float acc = 0;
        if (projection > 0) {
            acc = glm::clamp(distance / Bird::params.max_back_distance, 0.0f, 1.0f);
            if (boid_speed < head_speed)acc *= 2.0f;
        }
        else if(projection < 0){
            acc = -glm::clamp(distance / Bird::params.max_back_distance, 0.0f, 1.0f);
            if (boid_speed > head_speed)acc *= 2.0f;
        }

        totalForce = separation * dymatic_separation_param + cohesion * dymatic_cohension_param + alignment * dymatic_alignment_param + boid_dir * acc;

        boid.ApplyForce(totalForce);
    }
}


void applyShapeForce(float delta_time, std::vector<Bird>& boids, HeadBird& headBird) {
    if (headBird.velocity == glm::vec3(0.0f))return;

    glm::mat4 head_rotation = headBird.ToRotateMartix();
    glm::vec3 head_dir = glm::normalize(headBird.velocity);
    for (auto &boid : boids) {


        glm::vec3 target_position = glm::vec3(head_rotation * glm::vec4(boid.targetPosition, 1.0f)) +headBird.position;

        

        float distance = glm::length(boid.position - target_position);
        glm::vec3 direction = glm::normalize(target_position - boid.position);
        glm::vec3 total_Force(0.0f);
        if (distance > Bird::params.min_force_distance) {
            float dymatic_shape_force_weight = Bird::params.shape_force_weight;
            if (distance > Bird::params.max_force_distance)dymatic_shape_force_weight *= 5.0f;
            glm::vec3 desired_velocity = glm::normalize(direction) * boid.params.max_speed;

            // ����ת����
            glm::vec3 steer = desired_velocity - boid.velocity;

            // Ӧ����
            total_Force += steer * dymatic_shape_force_weight;

        }
        else {
            total_Force += -(glm::normalize(boid.velocity) * Bird::params.shape_force_weight);
        }

        float angle = angleBetweenVectors(head_dir, boid.velocity);

        if (angle > 0.5f) {
            glm::vec3 alignment = head_dir;//��������

            float dymatic_alignment_param = Bird::params.head_alignment_weight;

            if (angle > 3.0f)dymatic_alignment_param *= 2.0f;

            total_Force += head_dir * dymatic_alignment_param;
        }

        boid.ApplyForce(total_Force);

    }
}

void getBoidsUpdate(float delta_time, std::vector<Bird>& boids) {
    for (auto& boid : boids) {
        boid.BoundaryCheck();
        boid.UpdateSelf(delta_time);
    }
}

void BirdSystem::update(float delta_time,std::vector<Bird>& birds) {
    // ��һ�׶Σ���������BOID����Ϊ
    applyThreeRules(birds);

    // �ڶ��׶Σ���������BOID��λ��
    getBoidsUpdate(delta_time, birds);
}


void BirdSystem::update(float delta_time, std::vector<Bird>& birds,HeadBird& headBird) {
    // ��һ�׶Σ���������BOID����Ϊ
    applyThreeRules(birds);
    applyHeadBirdForceWithNoShape(delta_time, birds, headBird);
    // �ڶ��׶Σ���������BOID��λ��
    getBoidsUpdate(delta_time, birds);
    headBird.UpdateSelf(delta_time);
}

void BirdSystem::update(float delta_time, std::vector<Bird>& birds, HeadBird& headBird,BirdShape &shape) {
    // ��һ�׶Σ���������BOID����Ϊ

    applyShapeForce(delta_time, birds, headBird);
    // �ڶ��׶Σ���������BOID��λ��
    getBoidsUpdate(delta_time, birds);
    headBird.UpdateSelf(delta_time);
}
