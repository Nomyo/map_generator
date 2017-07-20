#pragma once

#include <camera.hh>
#include <mesh-terrain.hh>
#include <light.hh>
#include <terrain-texture-pack.hh>

class TerrainRenderer
{
public:
    TerrainRenderer(Shader shader, glm::mat4 projection_mat,
		    glm::mat4 view_mat, glm::vec3 view_pos, Light light);
    ~TerrainRenderer() = default;
    void render(const MeshTerrain& terrain, TerrainTexturePack* t_pack);
    void prepare_instance(TerrainTexturePack* t_pack);

private:
    Shader shader_;
};
