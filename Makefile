CXX = g++
CXXFLAGS = -Wall -std=c++14 -pedantic -g
LDFLAGS = -lGLEW -lglfw3 -lGL -lm -ldl -lXinerama \
	-lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSDL2main -lSDL2 \
	-lglut -lSDL2_image -lGL -lGLU -lassimp

DIR = src
SOURCE = \
	$(DIR)/glad.c 			\
	$(DIR)/utils.cc			\
	$(DIR)/simplexnoise.cc 		\
	$(DIR)/camera.cc		\
	$(DIR)/stb_image.cc		\
	$(DIR)/mesh-terrain.cc 		\
	$(DIR)/input.cc			\
	$(DIR)/opengl-utils.cc		\
	$(DIR)/shader_m.cc 		\
	$(DIR)/entity.cc		\
	$(DIR)/model.cc			\
	$(DIR)/entity-renderer.cc	\
	$(DIR)/mesh-texture.cc		\
	$(DIR)/light.cc			\
	$(DIR)/terrain-renderer.cc      \
	$(DIR)/main.cc

INC_DIR = inc/
OBJ = $(SOURCE:%.cc=%.o)
OBJc = $(SOURCE:%.c=%.o)

EXECUTABLE = bin

all: $(OBJc) $(OBJ) $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)
clean:
	$(RM) $(DIR)/*.o $(EXECUTABLE)

.PHONY: clean
