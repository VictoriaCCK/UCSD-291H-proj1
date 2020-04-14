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
    }
//    {
//        // Front
//        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),
//        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
////        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z),
//        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),

////         Back
//        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
//        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
//        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),
//        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
//
////         Top
//        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),
//        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z),
//        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
//        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),
//
////         Bottom
//        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
//        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
//        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
//        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),

        // Left
//        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
//        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),
//        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),
//        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),
//
////         Right
//        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
//        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
//        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
//        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z)
//    };

    // Specify normals
//    normals = {
//        // Front
//        glm::vec3(0,0,1),
//        glm::vec3(0,0,1),
////        glm::vec3(0,0,1),
//        glm::vec3(0,0,1),
//
//        // Back
//        glm::vec3(0,0,-1),
//        glm::vec3(0,0,-1),
//        glm::vec3(0,0,-1),
//        glm::vec3(0,0,-1),

        // Top
//        glm::vec3(0,1,0),
//        glm::vec3(0,1,0),
//        glm::vec3(0,1,0),
//        glm::vec3(0,1,0),

        // Bottom
//        glm::vec3(0,-1,0),
//        glm::vec3(0,-1,0),
//        glm::vec3(0,-1,0),
//        glm::vec3(0,-1,0),
//
//        // Left
//        glm::vec3(-1,0,0),
//        glm::vec3(-1,0,0),
//        glm::vec3(-1,0,0),
//        glm::vec3(-1,0,0),

//        // Right
//        glm::vec3(1,0,0),
//        glm::vec3(1,0,0),
//        glm::vec3(1,0,0),
//        glm::vec3(1,0,0)
//    };


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
        0,1,2,        0,2,3,            // Front
        0,1,3,        1,2,3,
//        4,5,6,        4,6,7,            // Back
//        8,9,10,        8,10,11,        // Top
//        12,13,14,    12,14,15,        // Bottom
//        16,17,18,    16,18,19,        // Left
//        20,21,22,    20,22,23,        // Right
    };

    construct_T();
//    glm::vec3 a = cross((points[2].position - points[0].position), (points[1].position - points[0].position));
//    glm::vec3 b = points[3].position - points[0].position;
//    std::cout<<" vec a is "<<a[0]<<" "<<a[1]<<" "<<a[2]<<std::endl;
//    std::cout<<" vec b is "<<b[0]<<" "<<b[1]<<" "<<b[2]<<std::endl;
    vol = dot(cross((points[2].position - points[0].position), (points[1].position - points[0].position)), (points[3].position - points[0].position)) / 6;
    mass = destiny * vol;
    for(int i = 0; i<p.size(); i++) Particles[p[i]]->mass = fabs(mass / 4);
//    std::cout<<" mass is "<<mass<<std::endl;
    R = T;
//
//// debug for print T R
////    for (int i = 0; i<3; i++) {
////        for (int j = 0; j<3; j++) {
////            std::cout<<T[i][j]<<" ";
////        }
////        std::cout<<std::endl;
////    }
////
////    std::cout<<std::endl;
//
//   glm::mat3 IR = glm::inverse(R);
//
////    for (int i = 0; i<3; i++) {
////        for (int j = 0; j<3; j++) {
////            std::cout<<IR[i][j]<<" ";
////        }
////        std::cout<<std::endl;
////    }
//
//    F = dot_matrix(T, IR);
//
////    for (int i = 0; i<3; i++) {
////        for (int j = 0; j<3; j++) {
////            std::cout<<F[i][j]<<" ";
////        }
////        std::cout<<std::endl;
////    }
//
//    glm::mat3 TF = glm::transpose(F);
//    strain_tensor = (dot_matrix(TF, F) - glm::mat3(1));
//
//    for (int i = 0; i<3; i++) {
//        for (int j = 0; j<3; j++) {
//            strain_tensor[i][j] *= 0.5;
//        }
//    }
    construct_strain_tensor();
    construct_stress_tensor();
//    std::cout<<" cal stress_tensor is "<<stress_tensor[0][0]<<std::endl;
//    for (int i = 0; i < points.size(); i++) {
//        Particles[p[i]]->apply_force((dot_matrix_vector(dot_matrix(F, stress_tensor), glm::normalize(normals[i]))));
//        std::cout<<p[i]<<"hey cal force is "<<Particles[p[i]]->force[0]<<Particles[p[i]]->force[1]<<Particles[p[i]]->force[2]<<std::endl;
//    }


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
    
    //    glm::vec3 a = cross((points[2].position - points[0].position), (points[1].position - points[0].position));
    //    glm::vec3 b = points[3].position - points[0].position;
    //    std::cout<<" vec a is "<<a[0]<<" "<<a[1]<<" "<<a[2]<<std::endl;
    //    std::cout<<" vec b is "<<b[0]<<" "<<b[1]<<" "<<b[2]<<std::endl;
    vol = dot(cross((points[2].position - points[0].position), (points[1].position - points[0].position)), (points[3].position - points[0].position)) / 6;
    mass = destiny * vol;
    for(int i = 0; i<point_idx.size(); i++) Particles[point_idx[i]]->mass = fabs(mass / 4);
    //    std::cout<<" mass is "<<mass<<std::endl;
    //// debug for print T R
    ////    for (int i = 0; i<3; i++) {
    ////        for (int j = 0; j<3; j++) {
    ////            std::cout<<T[i][j]<<" ";
    ////        }
    ////        std::cout<<std::endl;
    ////    }
    ////
    ////    std::cout<<std::endl;
    //
    //   glm::mat3 IR = glm::inverse(R);
    //
    ////    for (int i = 0; i<3; i++) {
    ////        for (int j = 0; j<3; j++) {
    ////            std::cout<<IR[i][j]<<" ";
    ////        }
    ////        std::cout<<std::endl;
    ////    }
    //
    //    F = dot_matrix(T, IR);
    //
    ////    for (int i = 0; i<3; i++) {
    ////        for (int j = 0; j<3; j++) {
    ////            std::cout<<F[i][j]<<" ";
    ////        }
    ////        std::cout<<std::endl;
    ////    }
    //
    //    glm::mat3 TF = glm::transpose(F);
    //    strain_tensor = (dot_matrix(TF, F) - glm::mat3(1));
    //
    //    for (int i = 0; i<3; i++) {
    //        for (int j = 0; j<3; j++) {
    //            strain_tensor[i][j] *= 0.5;
    //        }
    //    }
    construct_T();
    construct_strain_tensor();
    construct_stress_tensor();
    //    std::cout<<" cal stress_tensor is "<<stress_tensor[0][0]<<std::endl;
        for (int i = 0; i < points.size(); i++) {
            Particles[point_idx[i]]->apply_force((dot_matrix_vector(dot_matrix(F, stress_tensor), normals[i] * (1 / glm::length(normals[i])))));
            std::cout<<point_idx[i]<<" cal force is "<<Particles[point_idx[i]]->force[0]<<Particles[point_idx[i]]->force[1]<<Particles[point_idx[i]]->force[2]<<std::endl;
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
    std::vector<glm::vec3> e;
    e.push_back(points[0].position - points[3].position);
    e.push_back(points[1].position - points[3].position);
    e.push_back(points[2].position - points[3].position);
    
    for (int i = 0; i<3; i++) {
        glm::vec3 v;
        for (int j = 0; j<e.size(); j++) {
            v[j] = e[j][i];
        }
        T[i] = v;
    }
//    std::cout<<" T "<<std::endl;
//    for (int i = 0; i<3; i++) {
//        for (int j = 0; j<3; j++) {
//            std::cout<<T[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
//    std::cout<<" e "<<std::endl;
//    for (int i = 0; i<e.size(); i++) {
//        for (int j = 0; j<3; j++) {
//            std::cout<<e[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
}

void Tetrahedral::construct_strain_tensor()
{
    // debug for print T R
//    std::cout<<"T"<<std::endl;
//        for (int i = 0; i<3; i++) {
//            for (int j = 0; j<3; j++) {
//                std::cout<<T[i][j]<<" ";
//            }
//            std::cout<<std::endl;
//        }
//
//        std::cout<<std::endl;
//    std::cout<<"IR"<<std::endl;
    glm::mat3 IR = glm::inverse(R);
//
//    for (int i = 0; i<3; i++) {
//        for (int j = 0; j<3; j++) {
//            std::cout<<IR[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
    
    F = dot_matrix(T, IR);
//    std::cout<<"F"<<std::endl;
//    for (int i = 0; i<3; i++) {
//        for (int j = 0; j<3; j++) {
//            std::cout<<F[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
    
    glm::mat3 TF = glm::transpose(F);
    strain_tensor = (dot_matrix(TF, F) - glm::mat3(1));
//    std::cout<<"strain"<<std::endl;
//    for (int i = 0; i<3; i++) {
//        for (int j = 0; j<3; j++) {
//            strain_tensor[i][j] *= 0.5;
//            std::cout<<strain_tensor[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
}

void Tetrahedral::construct_stress_tensor()
{
    float E = 10000, v = 0.5;
    float lambda = E /((1 + v) * (1 - 2 * v));
    float mu = E /((1 + v) * 2);
//    std::cout<<" cal lam m is "<<lambda<<" "<<mu<<std::endl;
    stress_tensor = 2 * mu * strain_tensor + lambda * trace(strain_tensor) * glm::mat3(1);
//        std::cout<<"stress"<<std::endl;
//        for (int i = 0; i<3; i++) {
//            for (int j = 0; j<3; j++) {
//                stress_tensor[i][j] *= 0.5;
//                std::cout<<stress_tensor[i][j]<<" ";
//            }
//            std::cout<<std::endl;
//        }
}

float Tetrahedral::trace(glm::mat3 a)
{
    return a[0][0] * a[0][0] + a[1][1] * a[1][1] + a[2][2] * a[2][2];
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
