#include <opengl-utils.hh>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

MeshTerrain* create_mesh_from_noise(int startZ, int startX, int lengthZ, int lengthX)
{
    SimplexNoise noise_generator;
    SimplexNoise noise_generator2;
    std::vector<Vertex> vertices = create_vertices_from_noise(startZ, startX, lengthZ, lengthX);
    std::vector<unsigned int> indices;

    for (int z = 0; z < lengthZ - 1; ++z)
    {
    	for (int x = 0; x < lengthX - 1; ++x)
    	{
    	    int start = x + z * lengthZ;
    	    indices.push_back(start);
    	    indices.push_back(start + 1);
    	    indices.push_back(start + lengthZ);
    	    indices.push_back(start + 1);
    	    indices.push_back(start + 1 + lengthZ);
    	    indices.push_back(start + lengthZ);
    	}
    }

    return new MeshTerrain(vertices, indices);
}

MeshTerrain* create_mesh_from_noise(int startZ, int startX, int lengthZ, int lengthX, std::vector<Vertex> vertices)
{
    std::vector<unsigned int> indices;

    for (int z = 0; z < lengthZ - 1; ++z)
    {
    	for (int x = 0; x < lengthX - 1; ++x)
    	{
    	    int start = x + z * lengthZ;
    	    indices.push_back(start);
    	    indices.push_back(start + 1);
    	    indices.push_back(start + lengthZ);
    	    indices.push_back(start + 1);
    	    indices.push_back(start + 1 + lengthZ);
    	    indices.push_back(start + lengthZ);
    	}
    }

    return new MeshTerrain(vertices, indices);
}

std::vector<Vertex> create_vertices_from_noise(int startZ, int startX, int lengthZ, int lengthX)
{
    SimplexNoise noise_generator;
    SimplexNoise noise_generator2;
    std::vector<Vertex> vertices;

    for (int z = startZ; z < startZ + lengthZ; ++z)
    {
    	for (int x = startX; x < startX + lengthX; ++x)
    	{
    	    double scale = 0.003;
    	    float p_noise =
          noise_generator.sum_octave(16, x, z, 0.6, scale, 0, 255);
    	    double scale2 = 0.005;
    	    float m_noise =
    		  noise_generator2.sum_octave(16, x, z, 0.6, scale2, 0, 255);

    	    glm::vec3 color;
    	    glm::vec3 blend_color;

    	    set_color_from_noise(p_noise, m_noise, color, blend_color);

    	    vertices.emplace_back(
    		  Vertex{glm::vec3(x, (p_noise / 2.0f) * 0.5f , z),
    			color, glm::vec2{x, z},
    			blend_color});
    	}
    }

    return vertices;
}

std::vector<Vertex> create_vertices_from_noise(int startZ, int startX, int lengthZ, int lengthX, int seed)
{
    SimplexNoise noise_generator(seed);
    SimplexNoise noise_generator2(seed + 1);
    std::vector<Vertex> vertices;

    for (int z = startZ; z < startZ + lengthZ; ++z)
    {
    	for (int x = startX; x < startX + lengthX; ++x)
    	{
    	    double scale = 0.003;
    	    float p_noise =
          noise_generator.sum_octave(16, x, z, 0.6, scale, 0, 255);
    	    double scale2 = 0.005;
    	    float m_noise =
    		  noise_generator2.sum_octave(16, x, z, 0.6, scale2, 0, 255);

    	    glm::vec3 color;
    	    glm::vec3 blend_color;

    	    set_color_from_noise(p_noise, m_noise, color, blend_color);

    	    vertices.emplace_back(
    		  Vertex{glm::vec3(x, (p_noise / 2.0f) * 0.5f , z),
    			color, glm::vec2{x, z},
          blend_color});
    	}
    }

    return vertices;
}

std::vector<Vertex> create_vertices_from_flat(int startZ, int startX, int lengthZ, int lengthX, int y)
{
    std::vector<Vertex> vertices;

    for (int z = startZ; z < startZ + lengthZ; z += 1)
    {
    	for (int x = startX; x < startX + lengthX; x += 1)
    	{
    	    glm::vec3 color{1.f};
    	    glm::vec3 blend_color{1.f};

    	    vertices.emplace_back(
    		  Vertex{glm::vec3(x, y , z),
    			color, glm::vec2{x, z},
          blend_color});
    	}
    }

    return vertices;
}

std::vector<Entity*> create_entities_from_vertices(const std::vector<Vertex>& ve, std::vector<Model*>* models)
{
    auto entities = std::vector<Entity*>{};

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 5000);
    std::uniform_int_distribution<> distr2(0, 30000);

    auto rand = std::bind(distr, eng);
    auto rand2 = std::bind(distr2, eng);

    for (auto vertex : ve)
    {
    	if (vertex.position.y >= 22.0f)
    	{
    	    auto r = rand();
    	    switch (r)
    	    {
      	    case 1: // FIXME SHOULD CARE ABOUT MAP ROTATION
          		entities.push_back(new Entity{ models->at(0), vertex.position
          			    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.10f});
          		break;
          	case 2:
          		entities.push_back(new Entity{ models->at(0), vertex.position
          			    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.15f});
          		break;
          	case 3:
          		entities.push_back(new Entity{ models->at(0), vertex.position
          			    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.20f});
          	default:
          		break;
    	    }

    	    auto grass_r = rand2();
    	    if (grass_r == 0 || grass_r == 1)
    		    entities.push_back(new Entity{ models->at(1), vertex.position,
    			    glm::vec3(1.0f, 0.0f, 0.0f), 1.0f});
    	    if (grass_r == 2)
    		    entities.push_back(new Entity{ models->at(2), vertex.position,
    			    glm::vec3(1.0f, 0.0f, 0.0f), 1.0f});
    	}
    }
    return entities;
}

unsigned int load_texturegl(const std::string& path)
{
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height,
				    &nrChannels, 0);
    if (data)
    {
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
    		     GL_UNSIGNED_BYTE, data);
    	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
	     std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    return texture_id;
}


void set_color_from_noise(float height, float moisture,
			  glm::vec3& color, glm::vec3& blend_color)
{
    if (height < 50) // dark water
    {
	     color = glm::vec3{0.0f, 0.0f, (1.0f / 255.0f) * 204.0f};
	      blend_color = glm::vec3{1.0f, 0.0f, 0.0f};
    }
    else if (height < 75) // light water
    {
	     color = glm::vec3{0.0f, (1.0f / 255.0f) * 128.0f, (1.0f / 255.0f) * 255.0f};
	      blend_color = glm::vec3{1.0f, 0.0f, 0.0f};
    }

    else if (height < 80) // beach
    {
	     color = glm::vec3{(1.0f / 255.0f) * 228.0f, (1.0f / 255.0f) * 198.0f,
			  (1.0f / 255.0f) * 169.0f};
	     blend_color = glm::vec3{1.0f, 0.0f, 0.0f};
    }
    else if (height < 85) // beach transition
    {
    	color = glm::vec3{(1.0f / 255.0f) * 228.0f, (1.0f / 255.0f) * 198.0f,
    			  (1.0f / 255.0f) * 169.0f};
    	if (moisture < 100)
    	    blend_color = glm::vec3{0.50f, 0.0f, 0.50f};
    	else if (moisture < 159.5)
    	    blend_color = glm::vec3{0.49f, 0.0f, 0.0f}; // grass x white
    	else
    	    blend_color = glm::vec3{0.51f, 0.0f, 0.0f}; // grass x green dark
    }

    else if (height < 150) // desert / grass / dark grass
    {
    	color = glm::vec3{1.0f, 1.0f, 1.0f};

    	if (moisture < 100)
    	    blend_color = glm::vec3{0.0f, 0.0f, 1.0f};
    	else if (moisture < 159.5)
    	    blend_color = glm::vec3{0.0f, 0.0f, 0.0f}; // grass x b
    	else
    	{
    	    blend_color = glm::vec3{0.0f, 0.0f, 0.0f}; // grass x green dark
    	    color = glm::vec3{0.0f, (1.0f / 255.0f) * 102.0f ,0.0f}; // green dark
    	}
    }

    else if (height < 160) // desert / grass / dark grass transition
    {
    	color = glm::vec3{1.0f, 1.0f, 1.0f};

    	if (moisture < 100)
    	    blend_color = glm::vec3{0.0f, 0.0f, 1.0f};
    	else if (moisture < 159.5)
    	    blend_color = glm::vec3{0.0f, 0.5f, 0.0f}; // grassy x b
    	else
    	{
    	    blend_color = glm::vec3{0.0f, 0.5f, 0.0f}; // grassy x green dark
    	    color = glm::vec3{0.0f, (1.0f / 255.0f) * 102.0f ,0.0f}; // green dark
    	}
    }

    else if (height < 200) // desert / grassy / dark grassy
    {
    	color = glm::vec3{1.0f, 1.0f, 1.0f};

    	if (moisture < 100)
    	    blend_color = glm::vec3{0.0f, 0.0f, 1.0f};
    	else if (moisture < 159.5)
    	    blend_color = glm::vec3{0.0f, 1.0f, 0.0f}; // grassy x b
    	else
    	{
    	    blend_color = glm::vec3{0.0f, 1.0f, 0.0f}; // grassy x green dark
    	    color = glm::vec3{0.0f, (1.0f / 255.0f) * 102.0f ,0.0f}; // green dark
    	}
    }

    else if (height < 210)  // desert / grassy / dark grassy transition
    {
    	color = glm::vec3{1.0f, 1.0f, 1.0f};

    	if (moisture < 100)
    	    blend_color = glm::vec3{0.0f, 0.0f, 1.0f};
    	else if (moisture < 159.5)
    	    blend_color = glm::vec3{0.49f, 0.51f, 0.0f}; // grassy x b
    	else
    	    blend_color = glm::vec3{0.51f, 0.49f, 0.0f}; // grassy x green dark
    }
    else
    {
    	color = glm::vec3{1.0f, 1.0f, 1.0f};
    	blend_color = glm::vec3{1.0f, 0.0f, 0.0f}; // grass x green dark
    }
}
