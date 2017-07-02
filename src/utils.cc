#include <utils.hh>

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

static void CreateOrthographicProjection(GLfloat width, GLfloat height)
{
    // I use a near plane value of -1, and a far plane value of 1,
    // which is what works best for 2D games.
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
}

SDL_Window *window_init(void)
{
  SDL_Window *window = NULL;

  // Initialization
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cout << "Initialization failed : " << SDL_GetError() << std::endl;
    SDL_Quit();
    return nullptr;
  }

  // Double Buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

  CreateOrthographicProjection(4.0, 3.0);

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

SDL_Surface *flip_surface(SDL_Surface *surface)
{
    int current_line, pitch;
    SDL_Surface * fliped_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   surface->w,surface->h,
                                   surface->format->BitsPerPixel,
                                   surface->format->Rmask,
                                   surface->format->Gmask,
                                   surface->format->Bmask,
                                   surface->format->Amask);

    SDL_LockSurface(surface);
    SDL_LockSurface(fliped_surface);

    pitch = surface->pitch;
    for (current_line = 0; current_line < surface->h; current_line ++)
    {
        memcpy(&((unsigned char* )fliped_surface->pixels)[current_line*pitch],
               &((unsigned char* )surface->pixels)[(surface->h - 1  -
                                                    current_line)*pitch],
               pitch);
    }

    SDL_UnlockSurface(fliped_surface);
    SDL_UnlockSurface(surface);
    return fliped_surface;
}

GLuint load_texture(const char *filename, bool useMipMap)
{
    GLuint glID;
    SDL_Surface * picture_surface = NULL;
    SDL_Surface *gl_surface = NULL;
    SDL_Surface * gl_fliped_surface = NULL;
    Uint32 rmask, gmask, bmask, amask;

    picture_surface = IMG_Load(filename);
    if (picture_surface == NULL)
        return 0;

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

    SDL_PixelFormat format = *(picture_surface->format);
    format.BitsPerPixel = 32;
    format.BytesPerPixel = 4;
    format.Rmask = rmask;
    format.Gmask = gmask;
    format.Bmask = bmask;
    format.Amask = amask;

    gl_surface = SDL_ConvertSurface(picture_surface,&format,SDL_SWSURFACE);

    gl_fliped_surface = flip_surface(gl_surface);

    glGenTextures(1, &glID);

    glBindTexture(GL_TEXTURE_2D, glID);


    if (useMipMap)
    {

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, gl_fliped_surface->w,
                          gl_fliped_surface->h, GL_RGBA,GL_UNSIGNED_BYTE,
                          gl_fliped_surface->pixels);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, gl_fliped_surface->w,
                     gl_fliped_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
                     gl_fliped_surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    SDL_FreeSurface(gl_fliped_surface);
    SDL_FreeSurface(gl_surface);
    SDL_FreeSurface(picture_surface);

    return glID;
}
