#include "SDL/SDL.h"
#include <iostream>

#include <simplexnoise.h>
#include <utils.h>

int main(int argc, char *argv[])
{
  SDL_Surface *screen = screen_init();
  //display_bmp("carte.bmp", screen);
  //wait_key();

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

  display_surface(surf, screen);
  wait_key();

  return 0;
}
