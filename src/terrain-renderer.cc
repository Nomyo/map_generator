#include <terrain-renderer.hh>

TerrainRenderer::TerrainRenderer(Shader shader, glm::mat4 projection_mat,
				 glm::mat4 view_mat, glm::vec3 view_pos, Light light)
    : shader_(shader)
{
    shader_.use();
    shader_.setMat4("projection", projection_mat);
    shader_.setMat4("view", view_mat);
    shader_.setVec3("lightPos", light.get_position());
    shader_.setVec3("lightColor", light.get_color());
    shader_.setVec3("skyColour", glm::vec3(1.0f, 1.0f, 1.0f));
}

void TerrainRenderer::render(const MeshTerrain& terrain, TerrainTexturePack text_pack)
{
    prepare_instance(text_pack);
    terrain.draw(shader_);
}

void TerrainRenderer::prepare_instance(TerrainTexturePack text_pack)
{
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f));
    shader_.setMat4("model", model);

    shader_.setInt("background", 0);
    shader_.setInt("rtexture", 1);
    shader_.setInt("gtexture", 2);
    shader_.setInt("btexture", 3);
}
