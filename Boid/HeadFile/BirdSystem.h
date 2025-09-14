#pragma once
#include<Bird.h>
#include<HeadBird.h>
#include<BirdShape.h>
#include<vector>

class BirdSystem {
public:
    // ��������ϵͳ
    void update(float delta_time, std::vector<Bird>& birds);
    void update(float delta_time, std::vector<Bird>& birds,HeadBird& headBird);
    void update(float delta_time, std::vector<Bird>& birds, HeadBird& headBird, BirdShape& shape);
};