#include <entity-renderer.hh>

EntityRenderer::EntityRenderer(Shader shader, glm::mat4 projection_mat,
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

void EntityRenderer::render(const std::vector<Entity>& entities)
{
    for (auto e : entities)
    {
	prepare_instance(e);
        e.get_model().draw(shader_);
    }
}

void EntityRenderer::prepare_instance(const Entity& entity)
{
    glm::mat4 model;
    model = glm::translate(model, entity.get_position());
    model = glm::scale(model, glm::vec3(entity.get_scale()));

    //FIXME rotation
    model = glm::rotate(model, glm::radians(0.0f), entity.get_rotation());
    if (entity.get_model().has_fake_lighting())
	shader_.setFloat("fake_lighting", 1.0f);
    else
	shader_.setFloat("fake_lighting", 0.0f);
    shader_.setMat4("model", model);
}
