//
//  Vertice.hpp
//  291hw1
//
//  Created by Chiu Chi Kwan on 2020/4/6.
//  Copyright © 2020 ChiuChiuKwan. All rights reserved.
//

#ifndef Vertice_hpp
#define Vertice_hpp

#include <stdio.h>
#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Vertice
{
public:
    glm::mat4 model = glm::mat4(1);
    // Vertice Information
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 force = glm::vec3(0, -1, 0);
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float mass = 10;
    
public:
    Vertice(glm::vec3 point);
    ~Vertice();
    void apply_force(glm::vec3 f) {force += f;}
    void integrate (float deltaTime);
    
};

////////////////////////////////////////////////////////////////////////////////

#endif /* Vertice_hpp */
