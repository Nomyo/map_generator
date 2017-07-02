#include <SDL2/SDL.h>
#include "GL/glew.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include <simplexnoise.hh>
#include <utils.hh>
#include <freefly.hh>

#define FPS 50

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

void DrawGL(FreeFlyCamera* camera, SDL_Window *window)
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    camera->look();

    glBegin(GL_QUADS);
    glColor3ub(255,0,0); //face rouge
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,1,1);

    glColor3ub(0,255,0); //face verte
    glVertex3d(1,-1,1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,1,-1);
    glVertex3d(1,1,1);
    glColor3ub(0,0,255); //face bleue
    glVertex3d(-1,-1,1);
    glVertex3d(-1,-1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,-1,1);
    glEnd();

    glFlush();
    SDL_GL_SwapWindow(window);
}

void loop(FreeFlyCamera *camera, SDL_Window *window)
{
    SDL_Event event;
    const Uint32 time_per_frame = 1000/FPS;
    Uint32 last_time,current_time,elapsed_time;
    Uint32 stop_time;

    last_time = SDL_GetTicks();

    while (true)
    {
	while(SDL_PollEvent(&event))
	{
	    switch(event.type)
	    {
	    case SDL_QUIT:
		return;
		break;
	    case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		    return;
		    break;
		default :
		    camera->OnKeyboard(event.key);
		}
		break;
	    case SDL_KEYUP:
		camera->OnKeyboard(event.key);
		break;
	    case SDL_MOUSEMOTION:
		camera->OnMouseMotion(event.motion);
		break;
	    case SDL_MOUSEWHEEL:
		camera->OnMouseWheel(event.wheel);
		break;
	    }
	}

	current_time = SDL_GetTicks();
	elapsed_time = current_time - last_time;
	last_time = current_time;

	camera->animate(elapsed_time);

	DrawGL(camera, window);
	camera->debug();

	stop_time = SDL_GetTicks();
	if ((stop_time - last_time) < time_per_frame)
	{
	    SDL_Delay(time_per_frame - (stop_time - last_time));
	}
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window = window_init();
    SDL_GLContext contextOpenGL = SDL_GL_CreateContext(window);

    if (contextOpenGL == 0)
    {
	std::cout << SDL_GetError() << std::endl;
	SDL_DestroyWindow(window);
	SDL_Quit();

	return -1;
    }

    //load_textures();
    // do_noise(window);
    // wait_window_close();

    auto camera = new FreeFlyCamera(glm::vec3{0, 0, 2});

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (double)1280 / 720, 1, 1000);
    glEnable(GL_DEPTH_TEST);

    loop(camera, window);

    SDL_GL_DeleteContext(contextOpenGL);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
