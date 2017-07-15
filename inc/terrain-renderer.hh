#include <camera.hh>
#include <mesh-terrain.hh>
#include <light.hh>

class TerrainRenderer
{
public:
    TerrainRenderer(Shader shader, glm::mat4 projection_mat,
		    glm::mat4 view_mat, glm::vec3 view_pos, Light light);
    ~TerrainRenderer() = default;
    void render(const MeshTerrain& terrain);
    void prepare_instance();

private:
    Shader shader_;
};
