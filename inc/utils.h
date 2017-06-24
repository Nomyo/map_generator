#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>

/**
 * Wait until any key is pressed
 */
void wait_key(void);

/**
 * Wait until window is closed
 */
void wait_window_close(void);

/**
 * Load and display a bmp image
 * @param filename should be the relative path to the file
 */
void display_bmp(std::string file_name, SDL_Window* window);

/**
 * Init and return an SDL_Window
 * For now hard coded 1280 x 720
 */
SDL_Window *window_init(void);

/**
 * Create an SDL_Surface with the specified size
 */
std::shared_ptr<SDL_Surface> create_surface(int w, int h);

/**
 * Set the pixel color of a the surface at [i][j]
 * @param pixel: set to this pixel color
 */
void put_pixel_color(std::shared_ptr<SDL_Surface> surface,
		     int x, int y, uint32_t pixel);


/**
 * Set the pixel color of a the surface at [i][j]
 * @param pixel: set to this pixel color
 */
void display_surface(std::shared_ptr<SDL_Surface> surface, SDL_Window* window);
