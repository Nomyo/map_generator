#include <opengl-utils.hh>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

MeshTerrain create_mesh_from_noise()
{
    SimplexNoise noise_generator;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int z = 0; z < 300; ++z)
    {
	for (int x = 0; x < 300; ++x)
	{
	    double scale = 0.00085;
	    float p_noise =
		noise_generator.sum_octave(16, x, z, 0.6, scale, 0, 255);

	    glm::vec3 color;
	if (p_noise < 50)
	{
	    color.x = 0;
	    color.y = 0;
	    color.z = (1.0f / 255.0f) * 204.0f;
	}
	else if (p_noise < 70)
	{
	    color.x = 0;
	    color.y = (1.0f / 255.0f) * 128.0f;
	    color.z = (1.0f / 255.0f) * 255.0f;

	}
	else if (p_noise < 100)
	{
	    color.x = 0;
	    color.y = (1.0f / 255.0f) * 204.0f;
	    color.z = (1.0f / 255.0f) * 204.0f;
	}
	else if (p_noise < 120)
	{
	    color.x = 0;
	    color.y = (1.0f / 255.0f) * 102.0f;
	    color.z = (1.0f / 255.0f) * 51.0f;
	}
	else if (p_noise < 140)
	{
	    color.x = 0;
	    color.y = (1.0f / 255.0f) * 204.0f;
	    color.z = (1.0f / 255.0f) * 102.0f;
	}
	else if (p_noise < 160)
	{
	    color.x = 0;
	    color.y = (1.0f / 255.0f) * 153.0f;
	    color.z = 0;
	}
	else if (p_noise < 200)
	{
	    color.x = (1.0f / 255.0f) * 255.0f;
	    color.y = (1.0f / 255.0f) * 255.0f;
	    color.z = (1.0f / 255.0f) * 255.0f;
	}
	else if (p_noise < 220)
	{
	    color.x = (1.0f / 255.0f) * 255.0f;
	    color.y = (1.0f / 255.0f) * 255.0f;
	    color.z = (1.0f / 255.0f) * 255.0f;
	}
	else
	{
	    color.x = 1.0f;
	    color.y = 1.0f;
	    color.z = 1.0f;
	}

	vertices.emplace_back(
	    Vertex{glm::vec3(x, (p_noise / 2.0f) * 0.5f , z), color, glm::vec2{x, z}});
	}
    }

    for (int z = 0; z < 300 - 1; ++z)
    {
	for (int x = 0; x < 300 - 1; ++x)
	{
	    int start = x + z * 300;
	    indices.push_back(start);
	    indices.push_back(start + 1);
	    indices.push_back(start + 300);
	    indices.push_back(start + 1);
	    indices.push_back(start + 1 + 300);
	    indices.push_back(start + 300);
	}
    }

    MeshTerrain m(vertices, indices);
    return m;
}

std::vector<Entity> create_entities_from_vertices(const std::vector<Vertex>& ve)
{
    // FIXME REALLY NOT AN OPTIMIZED SOLUTION
    // the right way should be, one model and multiple position
    // to display it. Do it later.

    auto entities = std::vector<Entity>{};
    Model tree_model("textures/pine.obj", "textures/pine.png", "", false);
    Model grass_model("textures/grassModel.obj", "textures/grassTexture.png", "", false);
    grass_model.set_fake_lighting(true);
    Model flower_model("textures/grassModel.obj", "textures/flower.png", "", false);
    flower_model.set_fake_lighting(true);

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 500);
    std::uniform_int_distribution<> distr2(0, 3000);

    auto rand = std::bind(distr, eng);
    auto rand2 = std::bind(distr2, eng);

    for (auto vertex : ve)
    {
	if (!(vertex.position.y >= 20.0f && vertex.position.y < 25.0f))
	{
	    auto r = rand();
	    switch (r)
	    {
	    case 1: // FIXME SHOULD CARE ABOUT MAP ROTATION
		entities.push_back(Entity{tree_model, vertex.position
			    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.30f});
		break;
	    case 2:
		entities.push_back(Entity{tree_model, vertex.position
			    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.45f});
		break;
	    case 3:
		entities.push_back(Entity{tree_model, vertex.position
			    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.60f});
	    default:
		break;
	    }

	    auto grass_r = rand2();
	    if (grass_r == 0 || grass_r == 1)
		entities.push_back(Entity{grass_model, vertex.position,
			    glm::vec3(1.0f, 0.0f, 0.0f), 1.0f});
	    if (grass_r == 2)
		entities.push_back(Entity{flower_model, vertex.position,
			    glm::vec3(1.0f, 0.0f, 0.0f), 1.0f});
	}
    }
    return entities;
}
