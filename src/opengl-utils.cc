#include <opengl-utils.hh>

/**
 ** Vectors 
 */
const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

inline void check_sea_border(const std::vector<std::vector<double>>& height_map, std::vector<int>& dir, int y, int x) 
{
  for (int i = 0; i < 4; ++i)
    if (height_map[y + dy[i]][x + dx[i]] >= 75)
      dir.push_back(i + 1);
}

inline bool check_border(const std::vector<std::vector<double>>& height_map, size_t y, size_t x)
{
  return y <= 0 || x <= 0 || x >= height_map[0].size() - 1 || y >= height_map.size() - 1; 
}

void create_river(std::vector<std::vector<double>>& height_map, int y, int x, int count)
{
  int new_height = 74;
  if (count <= 0 || check_border(height_map, y , x))
    return;
  height_map[y][x] = new_height;
  int score = height_map[y][x + 1];
  int dir = 1;
  for (int i = 1; i < 4; ++i)
    {
      int curr = height_map[y + dy[i]][x + dx[i]];
      if (score < curr)
        {
          score = curr;
          dir = i + 1;
        }
    }
  if (score < 75)
    return;
  switch (dir) {
  case 1 :
    height_map[y - 1][x] = new_height;
    height_map[y + 1][x] = new_height;
    create_river(height_map, y, x + 1, count - 1);
    break;
  case 2 :
    height_map[y - 1][x] = new_height;
    height_map[y + 1][x] = new_height;
    create_river(height_map, y, x - 1, count - 1);
    break;
  case 3 :
    height_map[y][x - 1] = new_height;
    height_map[y][x + 1] = new_height;
    create_river(height_map, y + 1, x, count - 1);
    break;
  default:
    height_map[y][x - 1] = new_height;
    height_map[y][x + 1] = new_height;
    create_river(height_map, y - 1, x, count - 1);
  }
}

MeshTerrain create_mesh_from_noise()
{
  SimplexNoise noise_generator;
  SimplexNoise noise_generator2;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::vector<double>> height_map;
  std::vector<std::vector<double>> moist_map;
   
  for (int z = 0; z < 300; ++z)
    {
      double scale = 0.003;
      double scale2 = 0.005;       
      height_map.push_back(std::vector<double>());
      moist_map.push_back(std::vector<double>());
      for (int x = 0; x < 300; ++x)
        {
          height_map[z].push_back(noise_generator.sum_octave(16, x, z, 0.6, scale, 0, 255));
          moist_map[z].push_back(noise_generator2.sum_octave(16, x, z, 0.6, scale2, 0, 255));
        }
    }

  /**
   ** River Generation
   **/
  std::vector<std::tuple<int, int>> borders;
  
  for (int y = 1; y < 300 - 1; ++y)
    {
      for (int x = 1; x < 300 - 1; ++x)
        {
          double h = height_map[y][x];
          if (h < 50 || h >= 75)
            continue;
          std::vector<int> dir;
          check_sea_border(height_map, dir, y, x);
          if (dir.size() == 0)
            continue;
          borders.push_back(std::make_tuple(x, y));
        }
    }

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 99);  
  std::uniform_int_distribution<> distr2(100, 300);  
  auto rand = std::bind(distr, eng);
  auto rand2 = std::bind(distr2, eng);
  for (auto b : borders)
    {
      if (rand() == 0)
        {
          int len = rand2();
          create_river(height_map, std::get<1>(b), std::get<0>(b), len);
        }
    }
    
  // set Height color
  for (int z = 0; z < 300; ++z)
    {
      for (int x = 0; x < 300; ++x)
        {
          float p_noise = height_map[z][x];
          float m_noise = moist_map[z][x];
          glm::vec3 color;
          glm::vec3 blend_color;

          set_color_from_noise(p_noise, m_noise, color, blend_color);

          vertices.emplace_back(
                                Vertex{glm::vec3(x, (p_noise / 2.0f) * 0.5f , z),
                                    color, glm::vec2{x, z},
                                    blend_color});
        }
    }
    
  // set Indices
  for (int z = 0; z < 300 - 1; ++z)
    {
      for (int x = 0; x < 300 - 1; ++x)
        {
          int start = x + z * 300;
          indices.push_back(start);
          indices.push_back(start + 1);
          indices.push_back(start + 300);
          indices.push_back(start + 1);
          indices.push_back(start + 1 + 300);
          indices.push_back(start + 300);
        }
    }

  MeshTerrain m(vertices, indices);
  return m;
}

std::vector<Entity> create_entities_from_vertices(const std::vector<Vertex>& ve)
{
  // FIXME REALLY NOT AN OPTIMIZED SOLUTION
  // the right way should be, one model and multiple position
  // to display it. Do it later.

  auto entities = std::vector<Entity>{};
  Model tree_model("textures/pine.obj", "textures/pine.png", "", false);
  Model grass_model("textures/grassModel.obj", "textures/grassTexture.png", "", false);
  grass_model.set_fake_lighting(true);
  Model flower_model("textures/grassModel.obj", "textures/flower.png", "", false);
  flower_model.set_fake_lighting(true);

  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 500);
  std::uniform_int_distribution<> distr2(0, 3000);

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
              entities.push_back(Entity{tree_model, vertex.position
                    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.30f});
              break;
            case 2:
              entities.push_back(Entity{tree_model, vertex.position
                    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.45f});
              break;
            case 3:
              entities.push_back(Entity{tree_model, vertex.position
                    ,glm::vec3(1.0f, 0.0f, 0.0f), 0.60f});
            default:
              break;
            }

          auto grass_r = rand2();
          if (grass_r == 0 || grass_r == 1)
            entities.push_back(Entity{grass_model, vertex.position,
                  glm::vec3(1.0f, 0.0f, 0.0f), 1.0f});
          if (grass_r == 2)
            entities.push_back(Entity{flower_model, vertex.position,
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
  /*if (moisture >= 499)
    {
      color = glm::vec3{0.0f, 0.0f, (1.0f / 255.0f) * 204.0f};
      blend_color = glm::vec3{1.0f, 0.0f, 0.0f};
    }
    else*/ if (height < 50) // dark water
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
