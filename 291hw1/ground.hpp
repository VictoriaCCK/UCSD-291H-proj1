//
//  ground.hpp
//  291hw1
//
//  Created by Chiu Chi Kwan on 2020/4/10.
//  Copyright © 2020 ChiuChiuKwan. All rights reserved.
//

#ifndef ground_hpp
#define ground_hpp

#include <stdio.h>
#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Ground
{
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    glm::mat4 model;
    glm::vec3 color;
    
    // Ground Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    
public:
    Ground(glm::vec3 cubeMin=glm::vec3(-100, -2.5, -10), glm::vec3 cubeMax=glm::vec3(100, 2.5, 10));
    ~Ground();
    
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
    
    void spin(float deg);
};

////////////////////////////////////////////////////////////////////////////////

#endif /* ground_hpp */
