#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include <shader_m.hh>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};

class MeshColor
{
public:
    MeshColor(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~MeshColor();

    void draw(Shader shader);
    std::vector<Vertex> get_vertices();

private:
    void setup_mesh();

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;

    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int VAO_;
};
