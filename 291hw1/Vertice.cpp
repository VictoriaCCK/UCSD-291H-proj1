//
//  Vertice.cpp
//  291hw1
//
//  Created by Chiu Chi Kwan on 2020/4/6.
//  Copyright © 2020 ChiuChiuKwan. All rights reserved.
//

#include "Vertice.hpp"
#include <iostream>
Vertice::Vertice(glm::vec3 point){
    position = point;
}

Vertice::~Vertice(){}

void Vertice::integrate(float deltaTime)
{
//    if (force[0] != NAN) {
//            std::cout<<" uuuu force is "<<" "<<force[0]<<" "<<force[1]<<" "<<force[2]<<std::endl;
//            std::cout<<" iiiiii mass is "<<mass<<std::endl;
//    }
//    if(fabs(force[0]) > 20 or fabs(force[1]) > 20 or fabs(force[2]) > 20 )
//    force = glm::vec3(0, -10 * mass, 0);
    glm::vec3 accel = force / mass;
//    std::cout<<" acc is "<<accel[0]<<" "<<accel[1]<<" "<<accel[2]<<std::endl;
//    std::cout<<" pre velocity is "<<velocity[0]<<" "<<velocity[1]<<" "<<velocity[2]<<std::endl;
    velocity += accel * deltaTime;
    velocity *= 0.99;
//    std::cout<<" velocity is "<<velocity[0]<<" "<<velocity[1]<<" "<<velocity[2]<<std::endl;
//    std::cout<<" pre position is "<<position[0]<<" "<<position[1]<<" "<<position[2]<<std::endl;
    position += velocity * deltaTime;
//    std::cout<<" position is "<<position[0]<<" "<<position[1]<<" "<<position[2]<<std::endl;
    force = glm::vec3(0, -10*mass, 0);
}

