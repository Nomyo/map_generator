#include <mesh-texture.hh>

MeshTexture::MeshTexture(std::vector<VertexTexture> vertices,
			 std::vector<unsigned int> indices,
			 std::vector<Texture> textures)
{
	    this->vertices_ = vertices;
	    this->indices_ = indices;
	    this->textures_ = textures;

	    setup_mesh();
}

void MeshTexture::draw(Shader shader)
{
    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < textures_.size(); i++)
    {
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

			std::stringstream ss;
			std::string number;
			std::string name = textures_[i].type;
			if(name == "texture_diffuse")
			    ss << diffuseNr++; // transfer unsigned int to stream
			else if(name == "texture_specular")
			    ss << specularNr++; // transfer unsigned int to stream
			else if(name == "texture_normal")
			    ss << normalNr++; // transfer unsigned int to stream
			else if(name == "texture_height")
			    ss << heightNr++; // transfer unsigned int to stream
			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}


void MeshTexture::setup_mesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(VertexTexture),
		 &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
		 &indices_[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture),
			  (void*)offsetof(VertexTexture, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexture),
			  (void*)offsetof(VertexTexture, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture),
			  (void*)offsetof(VertexTexture, Tangent));

    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture),
			  (void*)offsetof(VertexTexture, Bitangent));

    glBindVertexArray(0);
}
