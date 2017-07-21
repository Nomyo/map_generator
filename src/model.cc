#include <model.hh>

Model::Model(const std::string& model_path, bool gamma)
    : gamma_correction(gamma)
{
    load_model(model_path);
}

Model::Model(const std::string& model_path,
	     const std::string& texture_path, const std::string& directory,
	     bool gamma)
    : model_path_(model_path)
    , texture_path_(texture_path)
    , directory_(directory)
    , gamma_correction(gamma)
{
    load_model(model_path);
}


void Model::draw(Shader shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
	meshes[i].draw(shader);
}

bool Model::has_fake_lighting()
{
    return fake_light_;
}

void Model::set_fake_lighting(bool b)
{
    fake_light_ = b;
}

void Model::load_model(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
					     aiProcess_Triangulate |
					     aiProcess_FlipUVs |
					     aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
       !scene->mRootNode)
    {
    	std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString()
    		  << std::endl;
    	return;
    }

    if (!texture_path_.size()) // FIXME ugly hack
	     directory_ = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
    	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    	meshes.push_back(processMesh(mesh, scene));
    }

    // after we've processed all of the meshes (if any)
    // we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
	       processNode(node->mChildren[i], scene);
    }
}

MeshTexture Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<VertexTexture> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
    	VertexTexture vertex;
    	glm::vec3 vector;

    	// positions
    	vector.x = mesh->mVertices[i].x;
    	vector.y = mesh->mVertices[i].y;
    	vector.z = mesh->mVertices[i].z;
    	vertex.Position = vector;

    	// normals
    	vector.x = mesh->mNormals[i].x;
    	vector.y = mesh->mNormals[i].y;
    	vector.z = mesh->mNormals[i].z;
    	vertex.Normal = vector;

    	// texture coordinates
    	if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    	{
    	    glm::vec2 vec;
    	    vec.x = mesh->mTextureCoords[0][i].x;
    	    vec.y = mesh->mTextureCoords[0][i].y;
    	    vertex.TexCoords = vec;
    	}
    	else
    	    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

    	vector.x = mesh->mTangents[i].x;
    	vector.y = mesh->mTangents[i].y;
    	vector.z = mesh->mTangents[i].z;
    	vertex.Tangent = vector;

    	vector.x = mesh->mBitangents[i].x;
    	vector.y = mesh->mBitangents[i].y;
    	vector.z = mesh->mBitangents[i].z;
    	vertex.Bitangent = vector;
    	vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
    	aiFace face = mesh->mFaces[i];

    	// retrieve all indices of the face and store them in the indices vector
    	for(unsigned int j = 0; j < face.mNumIndices; j++)
    	    indices.push_back(face.mIndices[j]);
        }

        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps =
    	loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps =
    	loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps =
    	loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps =
    	loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        if (textures_loaded.size() == 0 && texture_path_.size() != 0)
        {
    	Texture texture;
    	texture.id = TextureFromFile(texture_path_,
    				     gamma_correction);
    	texture.type = aiTextureType_DIFFUSE;
    	texture.path = texture_path_;
    	textures.push_back(texture);
    	textures_loaded.push_back(texture);
    }

    return MeshTexture(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
						 aiTextureType type,
						 std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
    	aiString str;
    	mat->GetTexture(type, i, &str);

    	bool skip = false;
    	for(unsigned int j = 0; j < textures_loaded.size(); j++)
    	{
    	    if(std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
    	    {
        		textures.push_back(textures_loaded[j]);
        		skip = true;
        		break;
    	    }
    	}

    	if(!skip)
    	{   // if texture hasn't been loaded already, load it
    	    Texture texture;
    	    std::string file(str.C_Str());
    	    texture.id = TextureFromFile(file + directory_,
    					 gamma_correction);
    	    texture.type = typeName;
    	    texture.path = str;
    	    textures.push_back(texture);
    	    textures_loaded.push_back(texture);
    	}
    }
    return textures;
}


unsigned int TextureFromFile(const std::string& filename, bool gamma)
{
    std::cout << filename << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
	GLenum format;
	if (nrComponents == 1)
	    format = GL_RED;
	else if (nrComponents == 3)
	    format = GL_RGB;
	else if (nrComponents == 4)
	    format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
    }
    else
    {
	std::cout << "Texture failed to load at path: " << filename << std::endl;
	stbi_image_free(data);
    }

    return textureID;
}
