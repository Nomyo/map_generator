#include <SDL2/SDL.h>
#include "GL/glew.h"
#include <iostream>

#include <simplexnoise.h>
#include <utils.h>

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

  //display_bmp("carte.bmp", window);

  SimplexNoise noise_generator;
  uint32_t mat[1280][720] = { 0 };

  for (int i = 0; i < 1280; ++i)
    for (int j = 0; j < 720; ++j)
    {
      // i * scale, j * scale) + 1) / 2.0f * 255.0;
      double scale = 0.003;
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

  wait_window_close();

  SDL_GL_DeleteContext(contextOpenGL);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
