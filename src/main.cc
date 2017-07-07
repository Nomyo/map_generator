#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include <camera.hh>
#include <input.hh>
#include <opengl-utils.hh>
#include <simplexnoise.hh>
#include <shader_m.hh>
#include <stb_image.h>
#include <utils.hh>
#include <main.hh>

void do_noise(SDL_Window *window)
{
    //display_bmp("carte.bmp", window);

    SimplexNoise noise_generator;
    uint32_t mat[1280][720] = { 0 };

    for (int i = 0; i < 1280; ++i)
	for (int j = 0; j < 720; ++j)
	{
	    double scale = 0.002;
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
    auto camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f),
			     glm::vec3(0.0f, 1.0f, 0.0f));

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
    Shader ourShader("shaders/basic.vs", "shaders/basic.fs");

    unsigned int VBO;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			  (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1;
    unsigned int texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height,
				    &nrChannels, 0);
    if (data)
    {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		     GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
	std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);

    // texture2
    // --------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
	std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


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

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// activate shader
	ourShader.use();

	// pass projection matrix to shader
	glm::mat4 projection = glm::perspective(
	    glm::radians(camera->get_zoom()),
	    (float)Input::SCR_WIDTH / (float)Input::SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = camera->get_view_matrix();
	ourShader.setMat4("view", view);

	// render boxes
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
	    glm::mat4 model;
	    model = glm::translate(model, cubePositions[i]);
	    float angle = 20.0f * i;
	    model = glm::rotate(model, glm::radians(angle),
				glm::vec3(1.0f, 0.3f, 0.5f));
	    ourShader.setMat4("model", model);

	    glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

int main(int argc, char *argv[])
{

    /**  SDL PART : OPEN WINDOW / PRINT NOISE AND PROCEDURAL RENDER  **/
    ////////////////////////////////////////////////////////////////////////
    // SDL_Window *window = window_init();				  //
    // SDL_GLContext contextOpenGL = SDL_GL_CreateContext(window);	  //
    // 									  //
    // if (contextOpenGL == 0)						  //
    // {								  //
    // 	std::cout << SDL_GetError() << std::endl;			  //
    // 	SDL_DestroyWindow(window);					  //
    // 	SDL_Quit();							  //
    // 									  //
    // 	return -1;							  //
    // }								  //
    // 									  //
    // do_noise(window);						  //
    // wait_window_close();						  //
    // 									  //
    // SDL_GL_DeleteContext(contextOpenGL);				  //
    // SDL_DestroyWindow(window);					  //
    // SDL_Quit();							  //
    ////////////////////////////////////////////////////////////////////////

    return start_opengl();
}
