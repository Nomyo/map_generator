#pragma once

#include <terrain-texture.hh>

class TerrainTexturePack
{
public:
    TerrainTexturePack(TerrainTexture backgroundTexture, TerrainTexture rTexture,
		       TerrainTexture gTexture, TerrainTexture bTexture)
	: backgroundTexture_(backgroundTexture)
	, rTexture_(rTexture)
	, gTexture_(gTexture)
	, bTexture_(bTexture) { }

    TerrainTexturePack() = default;

    TerrainTexture get_backgroundTexture() const { return backgroundTexture_; }
    TerrainTexture get_rTexture() const { return rTexture_; }
    TerrainTexture get_gTexture() const { return gTexture_; }
    TerrainTexture get_bTexture() const { return bTexture_; }

private:
    TerrainTexture backgroundTexture_;
    TerrainTexture rTexture_;
    TerrainTexture gTexture_;
    TerrainTexture bTexture_;
};
