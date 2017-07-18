#pragma once

#include <mesh-terrain.hh>
#include <entity.hh>
#include <simplexnoise.hh>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Whenever the window size change this callback function executes
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * Perform perlin noise and build a mesh with it
 */
MeshTerrain* create_mesh_from_noise(int startZ, int startX, int lengthZ, int lengthX);
MeshTerrain* create_mesh_from_noise(int startZ, int startX, int lengthZ, int lengthX, std::vector<Vertex> vertices);
std::vector<Vertex> create_vertices_from_noise(int startZ, int startX, int lengthZ, int lengthX);

/**
 * Fill up and entity for now randomly with trees
 */
std::vector<Entity> create_entities_from_vertices(const std::vector<Vertex>& ve);

/**
 * Load texture from a path and return its opengl id
 */
unsigned int load_texturegl(const std::string& path);

/**
 * Set color from height and moisture noise
 */
void set_color_from_noise(float height, float moisture,
			  glm::vec3& color, glm::vec3& blend_color);
