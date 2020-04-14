//
//  tetrahedral.hpp
//  291hw1
//
//  Created by Chiu Chi Kwan on 2020/4/5.
//  Copyright © 2020 ChiuChiuKwan. All rights reserved.
//

#ifndef tetrahedral_hpp
#define tetrahedral_hpp

#include <stdio.h>
#include <iostream>
#include "core.h"
#include "Vertice.hpp"

////////////////////////////////////////////////////////////////////////////////

class Tetrahedral
{
public:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    glm::mat4 model;
    glm::vec3 color;
    
    // Tetrahedral Information
    std::vector<Vertice> points;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<int> point_idx;
    glm::mat3 T;
    glm::mat3 R;
    glm::mat3 F;
    glm::mat3 strain_tensor;
    glm::mat3 stress_tensor;
    float vol = 0;
    float mass = 0;
    float destiny = 1000;
    
public:
    Tetrahedral(std::vector<int> p);
    ~Tetrahedral();
    
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
    
    void spin(float deg);
    void construct_T();
    void construct_strain_tensor();
    void construct_stress_tensor();
    glm::vec3 compute_force(int i);
    float trace(glm::mat3 a);
    glm::mat3 dot_matrix(glm::mat3 x, glm::mat3 y);
    glm::vec3 dot_matrix_vector(glm::mat3 x, glm::vec3 y);
};

#endif /* tetrahedral_hpp */
