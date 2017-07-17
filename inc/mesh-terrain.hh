
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include <shader_m.hh>
#include <terrain-texture-pack.hh>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coord;
    glm::vec3 blend_color;
    glm::vec3 normal;
};

class MeshTerrain
{
public:
    MeshTerrain(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    MeshTerrain(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
	TerrainTexturePack texture_pack);
    ~MeshTerrain();

    void draw(Shader shader) const;
    std::vector<Vertex> get_vertices() const;
    TerrainTexturePack get_texture_pack() const;
    void set_texture_pack(TerrainTexturePack t);

private:
    void setup_mesh();

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;

    TerrainTexturePack texture_pack_;

    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int VAO_;
};
