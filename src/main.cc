#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <array>

#include <camera.hh>
#include <entity-renderer.hh>
#include <input.hh>
#include <opengl-utils.hh>
#include <shader_m.hh>
#include <stb_image.h>
#include <utils.hh>
#include <mesh-terrain.hh>
#include <mesh-texture.hh>
#include <main.hh>
#include <model.hh>

void do_noise(SDL_Window *window)
{
    //display_bmp("carte.bmp", window);

    SimplexNoise noise_generator;
    uint32_t mat[1280][720] = { 0 };

    for (int i = 0; i < 1280; ++i)
	for (int j = 0; j < 720; ++j)
	{
	    double scale = 0.001;
	    int p_noise =
		noise_generator.sum_octave(16, i, j, 0.6, scale, 0, 255);
	    mat[i][j] = p_noise | (p_noise << 8) | (p_noise << 16);
	}

    auto surf = create_surface(1280, 720);
    for (int i = 0; i < 1280; ++i)
	for (int j = 0; j < 720; ++j)
	{
	    put_pixel_color(surf, i, j, mat[i][j]);
	}

    display_surface(surf, window);
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
			     glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.0f);

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

    // Mesh & Model
    MeshTerrain map_mesh = create_mesh_from_noise();

    //Entity
    auto map_vertices = map_mesh.get_vertices();
    auto entities = create_entities_from_vertices(map_vertices);

    our_map_shader.use();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// activate shader for map mesh ! !
	our_map_shader.use();

	glm::mat4 projection = glm::perspective(
	    glm::radians(camera->get_zoom()),
	    (float)Input::SCR_WIDTH / (float)Input::SCR_HEIGHT, 0.1f, 650.0f);
	our_map_shader.setMat4("projection", projection);

	glm::mat4 view = camera->get_view_matrix();
	our_map_shader.setMat4("view", view);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f),
			    glm::vec3(1.0f, 0.0f, 0.0f));
	our_map_shader.setMat4("model", model);
	map_mesh.draw(our_map_shader);

	// Render entities
	EntityRenderer z(our_model_shader, projection, view);
	z.render(entities);

	glfwSwapBuffers(window);
	glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int main(int argc, char *argv[])
{

    /**  SDL PART : OPEN WINDOW / PRINT NOISE AND PROCEDURAL RENDER  **/

    // SDL_Window *window = window_init();
    // SDL_GLContext contextOpenGL = SDL_GL_CreateContext(window);

    // if (contextOpenGL == 0)
    // {
    // 	std::cout << SDL_GetError() << std::endl;
    // 	SDL_DestroyWindow(window);
    // 	SDL_Quit();

    // 	return -1;
    // }

    // do_noise(window);
    // wait_window_close();

    // SDL_GL_DeleteContext(contextOpenGL);
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    return start_opengl();
}
