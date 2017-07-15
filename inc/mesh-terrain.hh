#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include <shader_m.hh>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coord;
};

class MeshTerrain
{
public:
    MeshTerrain(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~MeshTerrain();

    void draw(Shader shader) const;
    std::vector<Vertex> get_vertices() const;
    unsigned int get_texture_id() const;

private:
    void setup_mesh();
    void prepare_texture();

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;

    unsigned int texture_id_;

    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int VAO_;
};
