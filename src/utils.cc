#include <utils.h>

void wait_key(void)
{
  SDL_Event event;
  while (true)
    if (SDL_WaitEvent(&event))
      if (event.type == SDL_KEYDOWN)
	return;
}

void display_bmp(std::string file_name, SDL_Surface* screen)
{
  SDL_Surface *image;
  SDL_Surface *image_displayed;

  /* Load the BMP file into a surface */
  image = SDL_LoadBMP(file_name.c_str());
  if (image == NULL)
  {
    std::cerr << "Couldn't load " << file_name << " ;"
	      << SDL_GetError() << std::endl;
    return;
  }

  /*
   * Palettized screen modes will have a default palette (a standard
   * 8*8*4 colour cube), but if the image is palettized as well we can
   * use that palette for a nicer colour matching
   */
  if (image->format->palette && screen->format->palette) {
    SDL_SetColors(screen, image->format->palette->colors, 0,
		  image->format->palette->ncolors);
  }

  image_displayed = SDL_DisplayFormat(image);

  /* Blit onto the screen surface */
  if(SDL_BlitSurface(image_displayed, NULL, screen, NULL) < 0)
    std::cerr << "BlitSurface error: " << SDL_GetError() << std::endl;

  SDL_UpdateRect(screen, 0, 0, image_displayed->w, image_displayed->h);
  SDL_Flip(screen);

  /* Free the allocated BMP surface */
  SDL_FreeSurface(image);
}

SDL_Surface *screen_init(void)
{

  SDL_Surface *screen = NULL;

  /* Initialize the SDL library */
  if(SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }

  /* Clean up on exit */
  atexit(SDL_Quit);

  /*
   * Initialize the display in a 640x480 8-bit palettized mode,
   * requesting a software surface
   */
  /* Have a preference for 8-bit, but accept any depth */
  screen = SDL_SetVideoMode(1280, 720, 8, SDL_SWSURFACE | SDL_ANYFORMAT);
  if (screen == NULL )
  {
    std::cerr << "Couldn't set 640x480x8 video mode: " << SDL_GetError()
	      << std::endl;
    exit(1);
  }

  std::cout << "Set 640x480 at " << screen->format->BitsPerPixel
	    << "bits-per-pixel mode" << std::endl;
  return screen;
}

std::shared_ptr<SDL_Surface> create_surface(int width, int height)
{
  SDL_Surface *surface;
  uint32_t rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
  #else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
  #endif

  surface = SDL_CreateRGBSurface(0, width, height, 32,
				 rmask, gmask, bmask, amask);
  if (surface == NULL) {
    std::cerr << "SDL_CreateRGBSurface() failed: " << SDL_GetError() << std::endl;
    exit(1);
  }

  /* or using the default masks for the depth: */
  surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

  std::shared_ptr<SDL_Surface> tmp(surface);
  return tmp;
}


void put_pixel_color(std::shared_ptr<SDL_Surface> surface,
		     int x, int y, uint32_t pixel)
{
  int bpp = surface->format->BytesPerPixel;

  auto p = (unsigned char *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = (unsigned char)pixel;
    break;
  case 2:
    *(unsigned short*)p = (unsigned short)pixel;
    break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (unsigned char)((pixel >> 16) & 0xff);
      p[1] = (unsigned char)((pixel >> 8) & 0xff);
      p[2] = (unsigned char)(pixel & 0xff);
    } else {
      p[0] = (unsigned char)(pixel & 0xff);
      p[1] = (unsigned char)((pixel >> 8) & 0xff);
      p[2] = (unsigned char)((pixel >> 16) & 0xff);
    }
    break;
  case 4:
    *(unsigned long*)p = pixel;
    break;
  }
}

void display_surface(std::shared_ptr<SDL_Surface> surface, SDL_Surface* screen)
{
  if(SDL_BlitSurface(surface.get(), NULL, screen, NULL) < 0)
    std::cerr << "BlitSurface error: " << SDL_GetError() << std::endl;

  SDL_UpdateRect(screen, 0, 0, surface->w, surface->h);
  SDL_Flip(screen);
}
