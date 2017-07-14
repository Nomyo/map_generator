#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh-texture.hh>
#include <shader_m.hh>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const std::string &file,
			     bool gamma);

class Model
{
public:
    /*  Model Data */
    Model(const std::string& model_path, bool gamma = false);
    Model(const std::string& model_path,
	  const std::string& texture_path, const std::string& directory,
	  bool gamma);
    void draw(Shader shader);
    bool has_fake_lighting();
    void set_fake_lighting(bool b);

private:
    /*  Functions   */
    void load_model(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);

    MeshTexture processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given type
    // and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
					      aiTextureType type,
					      std::string typeName);
private:
    std::vector<Texture> textures_loaded;
    std::vector<MeshTexture> meshes;

    std::string model_path_;
    std::string texture_path_;
    std::string directory_;

    bool fake_light_ = false;
    bool gamma_correction;
};
