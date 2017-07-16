#pragma once

class TerrainTexture
{
public:
    TerrainTexture(unsigned int id)
	: texture_id_(id) { }
    TerrainTexture() = default;

    unsigned int get_texture_id() const { return texture_id_; }

private:
    unsigned int texture_id_;
};
