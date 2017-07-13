#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader_m.hh>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct VertexTexture {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class MeshTexture {
public:

    MeshTexture(std::vector<VertexTexture> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures);

    void draw(Shader shader);

private:
    void setup_mesh();

private:
    std::vector<VertexTexture> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};
