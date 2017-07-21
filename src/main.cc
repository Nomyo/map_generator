#include <main.hh>

void generate_chunk(std::vector<MeshTerrain*>* meshes, int seed, int startZ,
		    int startX, int lengthZ, int lengthX,
		    std::pair<int,int> chunk, std::vector<Model*>* models)
{
  //std::cout << startZ << "/" << startX << "/" << lengthZ << "/" << lengthX << std::endl;
  auto r = create_vertices_from_noise(startZ, startX, lengthZ, lengthX, seed);
  auto m = create_mesh_from_noise(0, 0, chunkZ, chunkX, r);
  m->set_chunk(chunk);

  auto tmp1 = create_vertices_from_flat(startZ, startX, lengthZ, lengthX, 19);
  auto waterMesh = create_mesh_from_noise(0, 0, chunkZ, chunkX, tmp1);

  g_mutex.lock();
  m->set_water(waterMesh);
  m->set_entities(create_entities_from_vertices(m->get_vertices(), models));

  meshes->push_back(m);
  g_mutex.unlock();
}

void manage_pool(std::vector<MeshTerrain*>* meshes,
		 std::vector<std::pair<int, int>>* unload,
		 Camera* cam, std::vector<Model*>* models)
{
  using namespace std::chrono_literals;
  std::random_device rd; // obtain a random number from hardware
  auto seed = rd();
  int i = 0;
  std::vector<std::thread> threadPool;
  std::vector<std::pair<int, int>> alreadyLoad;
  int size = 1;

  while(true)
  {
    g_mutex.lock();
    glm::vec3 view_pos = cam->get_view_pos();
    g_mutex.unlock();

    auto camPos = std::make_pair(((int)view_pos.z) / chunkZ, ((int)view_pos.x) / chunkX);

    // Search for chunks out of cam scope
    for (auto it = alreadyLoad.begin(); it != alreadyLoad.end();)
    {
        if (abs(it->first - camPos.first) > size || abs(it->second - camPos.second) > size)
        {
          g_mutex.lock();
          unload->push_back(*it);
          g_mutex.unlock();
          //std::cout << "Request remove of: " << it->first << "/" << it->second << std::endl;
          it = alreadyLoad.erase(it);
        } else {
          it++;
        }
    }

    for (int i = -size; i <= size; i++)
    {
      for (int j = -size; j <= size; j++)
      {
        if (std::find(alreadyLoad.begin(), alreadyLoad.end(),
		      std::make_pair(camPos.first + i, camPos.second + j)) == alreadyLoad.end())
        {
          // not found so load
          auto c = std::make_pair(camPos.first + i, camPos.second + j);
          threadPool.push_back(std::thread(generate_chunk, meshes, seed,
					   (camPos.first + i)*(chunkZ - 1),
					   (camPos.second + j)*(chunkX - 1),
					   chunkZ, chunkX, c, models));
          alreadyLoad.push_back(c);
        }
      }
    }
    for (auto& t: threadPool)
    {
      t.join();
    }
    threadPool.clear();
    i++;

    std::this_thread::sleep_for(1s);
  }
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int start_opengl()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Input::SCR_WIDTH, Input::SCR_HEIGHT,
					  "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
    	glfwTerminate();
    	return -1;
    }

    auto& input = Input::get_instance();
    auto camera = new Camera(glm::vec3(0.0f, 50.0f, 0.0f),
			     glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.f);

    input.init(camera);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, &Input::mouse_callback);
    glfwSetScrollCallback(window, &Input::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
    	std::cout << "Failed to initialize GLAD" << std::endl;
    	return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Create our shader
    Shader our_map_shader("shaders/basic.vs", "shaders/basic.fs");
    Shader our_model_shader("shaders/model.vs", "shaders/model.fs");
    Shader our_lamp_shader("shaders/lamp.vs", "shaders/lamp.fs");

    // Mesh & Model
    TerrainTexturePack* t_pack =
	new TerrainTexturePack(new TerrainTexture{load_texturegl("textures/grass.png")},
			      new TerrainTexture{load_texturegl("textures/Snow.jpg")},
			      new TerrainTexture{load_texturegl("textures/grassy3.png")},
			      new TerrainTexture{load_texturegl("textures/rocky.jpg")});

    std::vector<MeshTerrain*> map_mesh;
    std::vector<MeshTerrain*> meshes;
    std::vector<std::pair<int, int>> unload;
    std::vector<Model*>* models = new std::vector<Model*>
    {
      new Model("textures/pine.obj", "textures/pine.png", "", false),
      new Model("textures/grassModel.obj", "textures/grassTexture.png", "", false),
      new Model("textures/grassModel.obj", "textures/flower.png", "", false)
    };
    models->at(1)->set_fake_lighting(true);
    models->at(2)->set_fake_lighting(true);

    std::thread t(manage_pool, &meshes, &unload, camera, models);

    //Light
    Light map_light(glm::vec3(150.0f, 200.0f, 150.0f),
		    glm::vec3(1.0f, 1.0f, 1.0f));

    unsigned int VBO;
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    bool inc = false;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<std::string> faces
    {
        "textures/right.jpg",
        "textures/left.jpg",
        "textures/top.jpg",
        "textures/bottom.jpg",
        "textures/back.jpg",
        "textures/front.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // Skybox shader + load
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // water tests
    Shader waterShader("shaders/water.vs", "shaders/water.fs");
    Shader reflectiveShader("shaders/reflective.vs", "shaders/reflective.fs");
    unsigned int waterVAO, waterVBO;
    glGenVertexArrays(1, &waterVAO);
    glGenBuffers(1, &waterVBO);
    glBindVertexArray(waterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), &waterVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    float timer = 0.f;

    while (!glfwWindowShouldClose(window))
    {
      timer += 0.05f;

      // Process verts
      g_mutex.lock();
      for (auto& m: meshes)
      {
        m->set_texture_pack(t_pack);
        m->setup_mesh();
        m->get_water()->set_texture_pack(t_pack);
        m->get_water()->setup_mesh();

        map_mesh.push_back(m);
      }
      meshes.clear();

      for (auto& u: unload)
      {
        for (auto it = map_mesh.begin(); it != map_mesh.end();)
        {
          if ((*it)->get_chunk() == u)
          {
            delete *it;
            it = map_mesh.erase(it);
            break;
          } else {
            it++;
          }
        }
      }
      unload.clear();

      g_mutex.unlock();

    	// per-frame time logic
    	// --------------------
    	float currentFrame = glfwGetTime();
    	Input::deltaTime = currentFrame - Input::lastFrame;
    	Input::lastFrame = currentFrame;

    	// input
    	// -----
    	input.process_input(window);

    	// render
    	// ------
    	//glClearColor(149.0f / 255.0f,203.0f / 255.0f, 255.0f / 255.0f, 1.0f);
    	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// activate shader for map mesh ! !
    	glm::mat4 view = camera->get_view_matrix();
    	glm::mat4 projection = glm::perspective(glm::radians(camera->get_zoom()),
    	    (float)Input::SCR_WIDTH / (float)Input::SCR_HEIGHT, 0.1f, 1000.0f);
    	glm::vec3 view_pos = camera->get_view_pos();

    	// Render terrain
    	TerrainRenderer tr(our_map_shader, projection, view, view_pos, map_light);
      for (auto mesh: map_mesh)
      {
        tr.render(*mesh, t_pack);
      }

      EntityRenderer z(our_model_shader, projection, view, view_pos, map_light);
      for (auto mesh: map_mesh)
      {
        z.render(mesh->get_entities());
      }

    	// Display lamp
      our_lamp_shader.use();
      our_lamp_shader.setMat4("projection", projection);
      our_lamp_shader.setMat4("view", view);
    	glm::mat4 model;
    	model = glm::translate(model, map_light.get_position());
    	model = glm::scale(model, glm::vec3(3.0f)); // a smaller cube
      our_lamp_shader.setMat4("model", model);


    	glBindVertexArray(lightVAO);
    	glDrawArrays(GL_TRIANGLES, 0, 36);

      /*
      waterShader.use();
      waterShader.setFloat("time", timer);
      waterShader.setMat4("projection", projection);
      waterShader.setMat4("view", view);
      glm::mat4 model2;
      model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
      model2 = glm::rotate(model2, glm::radians(0.0f),
  			glm::vec3(1.0f, 0.0f, 0.0f));
      waterShader.setMat4("model", model2);

      // Enable blending
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      for (auto mesh: map_mesh)
      {
        mesh->get_water()->draw(waterShader);
      }
      */

      glm::mat4 model2;
      model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
      model2 = glm::rotate(model2, glm::radians(0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f));

      reflectiveShader.use();
      reflectiveShader.setFloat("time", timer);
      reflectiveShader.setMat4("projection", projection);
      reflectiveShader.setMat4("view", view);
      reflectiveShader.setMat4("model", model2);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      for (auto mesh: map_mesh)
      {
        mesh->get_water()->draw(reflectiveShader);
      }

      //glBindVertexArray(waterVAO);
    	//glDrawArrays(GL_TRIANGLES, 0, 9);

    	// update sun pos
    	auto light_pos = map_light.get_position();
    	if (light_pos.x > 450)
    	    inc = false;
    	if (light_pos.x < -150)
    	    inc = true;

    	map_light.set_position(glm::vec3(inc ? light_pos.x + 1 : light_pos.x - 1,
    					 light_pos.y, light_pos.z));
      //camera->process_keyboard(Camera::Camera_movement::FORWARD, Input::deltaTime * 10.f);

      glDepthFunc(GL_LEQUAL);
      skyboxShader.use();
      view = glm::mat4(glm::mat3(camera->get_view_matrix()));
      skyboxShader.setMat4("view", view);
      skyboxShader.setMat4("projection", projection);

      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int main(int argc, char *argv[])
{
    return start_opengl();
}
