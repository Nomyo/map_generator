#include <opengl-utils.hh>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Mesh create_mesh_from_noise()
{
    SimplexNoise noise_generator;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int z = 0; z < 300; ++z)
    {
	for (int x = 0; x < 300; ++x)
	{
	    double scale = 0.002;
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
	    color.z = (1.0f / 255.0f) * 102.0f;
	}
	else if (p_noise < 220)
	{
	    color.x = (1.0f / 255.0f) * 255.0f;
	    color.y = (1.0f / 255.0f) * 255.0f;
	    color.z = (1.0f / 255.0f) * 204.0f;
	}
	else
	{
	    color.x = 1.0f;
	    color.y = 1.0f;
	    color.z = 1.0f;
	}

	vertices.emplace_back(
	    Vertex{glm::vec3(x, -(p_noise / 2.0f), z),
		    color});
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

    Mesh m(vertices, indices);
    return m;
}
