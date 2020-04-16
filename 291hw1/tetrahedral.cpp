//
//  tetrahedral.cpp
//  291hw1
//
//  Created by Chiu Chi Kwan on 2020/4/5.
//  Copyright © 2020 ChiuChiuKwan. All rights reserved.
//

#include "tetrahedral.hpp"
#include "Window.h"
////////////////////////////////////////////////////////////////////////////////


Tetrahedral::Tetrahedral(std::vector<int> p)
{
    // Model matrix.
    model = glm::mat4(1.0f);
    // The color of the Tetrahedral. Try setting it to something else!
//    color = glm::vec3(1.0f, 0.95f, 0.1f);
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    // Specify vertex points
    point_idx = p;
    for (int i = 0; i<p.size(); i++) {
        points.push_back(*Particles[p[i]]);
        positions.push_back(Particles[p[i]]->position);
        pre_positions.push_back(Particles[p[i]]->position);
    }

    for (int i = 0; i<4; i++) {
        glm::vec3 l1, l2;
        switch (i) {
            case 0:{
                l1 = points[1].position - points[3].position;
                l2 = points[1].position - points[2].position;
                break;
            }

            case 1:{
                l1 = points[3].position - points[0].position;
                l2 = points[3].position - points[2].position;
                break;
            }

            case 2:{
                l1 = points[0].position - points[3].position;
                l2 = points[0].position - points[1].position;
                break;
            }

            case 3:{
                l1 = points[1].position - points[2].position;
                l2 = points[1].position - points[0].position;
                break;
            }

            default:
                break;
        }
        normals.push_back(cross(l1, l2));
    }


    // Specify indices
    indices = {
        0,1,2,        0,2,3,
        0,1,3,        1,2,3,
    };

    vol = dot(cross((points[0].position - points[3].position), (points[1].position - points[3].position)), (points[2].position - points[3].position)) / 6;
    mass = destiny * vol;
    for(int i = 0; i<p.size(); i++) Particles[p[i]]->mass += mass / 4;
    construct_T();
    R = T;
    construct_strain_tensor();
    construct_stress_tensor();
    for (int i = 0; i < points.size(); i++) {
        glm::vec3 f = F * stress_tensor * normals[i];
        f.operator*=(0.5);
        Particles[p[i]]->apply_force(f + 10 * Particles[p[i]]->mass);
    }


    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////////////

Tetrahedral::~Tetrahedral()
{
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

////////////////////////////////////////////////////////////////////////////////

void Tetrahedral::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tetrahedral::update()
{
    // Spin the Tetrahedral by 1 degree.
//    spin(0.1f);
    for (int i = 0; i<point_idx.size(); i++) {
        points[i] = *Particles[point_idx[i]];
        pre_positions[i] = positions[i];
        positions[i] = Particles[point_idx[i]]->position;
    }
    
    for (int i = 0; i<4; i++) {
        glm::vec3 l1, l2;
        switch (i) {
            case 0:{
                l1 = positions[1] - positions[3];
                l2 = positions[1] - positions[2];
                break;
            }

            case 1:{
                l1 = positions[3] - positions[0];
                l2 = positions[3] - positions[2];
                break;
            }

            case 2:{
                l1 = positions[0] - positions[3];
                l2 = positions[0] - positions[1];
                break;
            }

            case 3:{
                l1 = positions[1] - positions[2];
                l2 = positions[1] - positions[0];
                break;
            }

            default:
                break;
        }
        normals[i] = cross(l1, l2);
    }
//
//

    construct_T();
    construct_strain_tensor();
//    std::cout<<to_string(strain_tensor)<<std::endl;
    construct_stress_tensor();
        for (int i = 0; i < points.size(); i++) {
            glm::vec3 f = F * stress_tensor * normals[i];
//            std::cout<<"determinant is "<<glm::determinant(F)<<std::endl;
            f.operator*=(0.5);
            Particles[point_idx[i]]->apply_force(f);
//            if(point_idx[0] == 0)  {
//                std::cout<<" the force is "<<to_string(f)<<"point "<<point_idx[i]<<" is "<<to_string(Particles[point_idx[i]]->force)<<" the F is "<<to_string(F)<<"point "<<point_idx[i]<<" the stress_tensor is "<<to_string(stress_tensor)<<"point "<<" the normals is "<<to_string(normals[i])<<" the loc is"<<to_string(positions[i])<<std::endl;
//            }
        }
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO_positions);
    glBufferSubData(GL_ARRAY_BUFFER,0,positions.size() * sizeof(positions[0]),&positions[0]);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO_normals);
    glBufferSubData(GL_ARRAY_BUFFER,0,normals.size() * sizeof(normals[0]),&normals[0]);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

////////////////////////////////////////////////////////////////////////////////

void Tetrahedral::spin(float deg)
{
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}


////////////////////////////////////////////////////////////////////////////////

void Tetrahedral::construct_T()
{
    
//    glm::mat3 e;
//    e[0] = points[0].position - points[3].position;
//    e[1] = points[1].position - points[3].position;
//    e[2] = points[2].position - points[3].position;
    
    T[0] = positions[0] - positions[3];
    T[1] = positions[1] - positions[3];
    T[2] = positions[2] - positions[3];
    
//    T = glm::transpose(T);
//
//    std::cout<<" e0 is "<<to_string(positions[0] - positions[3])<<std::endl;
//    std::cout<<" T "<<std::endl;
//    std::cout<<to_string(T)<<std::endl;
}

void Tetrahedral::construct_strain_tensor()
{
    glm::mat3 IR = glm::inverse(R);

    F = T * IR;

    glm::mat3 TF = glm::transpose(F);
    strain_tensor = TF * F - glm::mat3(1);
    strain_tensor.operator*=(0.5);
}

void Tetrahedral::construct_stress_tensor()
{
    float E = 550, v = 0.2;
    float lambda = (E * v)/((1 + v) * (1 - 2 * v));
    float mu = E /((1 + v) * 2);
    stress_tensor = 2 * mu * strain_tensor + lambda * trace(strain_tensor) * glm::mat3(1);
}

float Tetrahedral::trace(glm::mat3 a)
{
    return a[0][0] + a[1][1] + a[2][2] ;
}

glm::mat3 Tetrahedral::dot_matrix(glm::mat3 x, glm::mat3 y)
{
    glm::mat3 res;
    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3 ; j++) {
            res[i][j] = 0;
            for (int k = 0; k<3; k++) {
                res[i][j] += x[i][k] * y[k][j];
            }
        }
    }
    return res;
}


glm::vec3 Tetrahedral::dot_matrix_vector(glm::mat3 x, glm::vec3 y)
{
    glm::vec3 res;
    
    for (int i = 0; i<3; i++) {
        res[i] = 0;
        for (int j = 0; j<3 ; j++) {
            res[i] += x[i][j] * y[j]; 
        }
    }
    
    return res;
}


glm::vec3 Tetrahedral::compute_force(int i)
{
    return (dot_matrix_vector(dot_matrix(F, stress_tensor), normals[i] * (1 / glm::length(normals[i]))));
}
