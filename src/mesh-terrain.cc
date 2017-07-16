#include <mesh-terrain.hh>
#include <opengl-utils.hh>

MeshTerrain::MeshTerrain(std::vector<Vertex> vertices,
		     std::vector<unsigned int> indices)
    : vertices_(vertices)
    , indices_(indices)
{
    setup_mesh();
}

MeshTerrain::MeshTerrain(std::vector<Vertex> vertices,
			 std::vector<unsigned int> indices,
			 TerrainTexturePack texture_pack)
    : vertices_(vertices)
    , indices_(indices)
    , texture_pack_(texture_pack)
{
    setup_mesh();
}

MeshTerrain::~MeshTerrain()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void MeshTerrain::draw(Shader /*shader*/) const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_pack_.get_backgroundTexture().get_texture_id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_pack_.get_rTexture().get_texture_id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_pack_.get_gTexture().get_texture_id());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture_pack_.get_bTexture().get_texture_id());

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<Vertex> MeshTerrain::get_vertices() const
{
    return vertices_;
}

TerrainTexturePack MeshTerrain::get_texture_pack() const
{
    return texture_pack_;
}

void MeshTerrain::set_texture_pack(TerrainTexturePack t)
{
    texture_pack_ = t;
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

    // text_coord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex),
			  (void*)(offsetof(Vertex, texture_coord)));

    // text_coord attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex),
			  (void*)(offsetof(Vertex, blend_color)));

   //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
