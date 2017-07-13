#include <mesh-color.hh>

MeshColor::MeshColor(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : vertices_(vertices)
    , indices_(indices)
{
    setup_mesh();
}

MeshColor::~MeshColor()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

void MeshColor::draw(Shader /*shader*/)
{
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<Vertex> MeshColor::get_vertices()
{
    return vertices_;
}


void MeshColor::setup_mesh()
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

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
