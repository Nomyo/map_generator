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

void TerrainRenderer::render(const MeshTerrain& terrain)
{
    prepare_instance();
    terrain.draw(shader_);
}

void TerrainRenderer::prepare_instance()
{
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f));
    shader_.setMat4("model", model);
}
