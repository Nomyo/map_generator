#include <utils.h>

void wait_key(void)
{
  SDL_Event event;
  while (true)
    if (SDL_WaitEvent(&event))
      if (event.type == SDL_KEYDOWN)
	return;
}

void wait_window_close(void)
{
  SDL_Event evenements;
  while(true)
  {
    SDL_WaitEvent(&evenements);

    if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
      break;
  }
}

void display_bmp(std::string file_name, SDL_Window* window)
{
   SDL_Surface *image;
   auto screen = SDL_GetWindowSurface(window);

   /* Load the BMP file into a surface */
   image = SDL_LoadBMP(file_name.c_str());
   if (image == NULL)
   {
     std::cerr << "Couldn't load " << file_name << " ;"
	       << SDL_GetError() << std::endl;
     return;
   }

   SDL_BlitSurface(image, NULL, screen, NULL); // blit it to the screen
   SDL_FreeSurface(image);
   SDL_UpdateWindowSurface(window);
}

SDL_Window *window_init(void)
{
  SDL_Window *window = NULL;

  // Initialization
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "Initialization failed : " << SDL_GetError() << std::endl;
    SDL_Quit();
    return nullptr;
  }

  // OpenGL version
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  // Double Buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  window = SDL_CreateWindow("Need title",
			     SDL_WINDOWPOS_CENTERED,
			     SDL_WINDOWPOS_CENTERED, 1280, 720,
			     SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if(window == 0)
  {
    std::cout << "Could not create the window : " << SDL_GetError() << std::endl;
    SDL_Quit();

    return nullptr;
  }

  return window;
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

void display_surface(std::shared_ptr<SDL_Surface> surface, SDL_Window* window)
{
  auto screen = SDL_GetWindowSurface(window);

  if(SDL_BlitSurface(surface.get(), NULL, screen, NULL) < 0)
    std::cerr << "BlitSurface error: " << SDL_GetError() << std::endl;

  SDL_UpdateWindowSurface(window);
}
