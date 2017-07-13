#include <mesh-terrain.hh>

MeshTerrain::MeshTerrain(std::vector<Vertex> vertices,
		     std::vector<unsigned int> indices)
    : vertices_(vertices)
    , indices_(indices)
{
    prepare_texture();
    setup_mesh();
}

MeshTerrain::~MeshTerrain()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void MeshTerrain::draw(Shader /*shader*/)
{
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<Vertex> MeshTerrain::get_vertices()
{
    return vertices_;
}

unsigned int MeshTerrain::get_texture_id()
{
    return texture_id_;
}

void MeshTerrain::prepare_texture()
{
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/grassFlowers.png", &width, &height,
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
}

void MeshTerrain::setup_mesh()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof (Vertex),
		 &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof (unsigned int),
		 &indices_[0], GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex),
			  (void*)(offsetof(Vertex, color)));


    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex),
			  (void*)(offsetof(Vertex, texture_coord)));


   //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
