#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include <shader_m.hh>

// For later use better with texture and light.
//////////////////////////////
// struct Vertex	    //
// {			    //
//     glm::vec3 position;  //
//     glm::vec3 normal;    //
//     glm::vec2 tex_coord; //
// };			    //
//////////////////////////////

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void draw(Shader shader);

private:
    void setup_mesh();

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;

    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int VAO_;
};
