CXX = g++
CXXFLAGS = -Wall -std=c++14 -pedantic
LDFLAGS = `sdl-config --cflags --libs` -lGLEW -lglfw3 -lGL -lm -ldl -lXinerama \
	-lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread

DIR = src
SOURCE = $(DIR)/utils.cc $(DIR)/simplexnoise.cc $(DIR)/main.cc
INC_DIR = inc/
OBJ = $(SOURCE:%.cc=%.o)

EXECUTABLE = bin

all: $(OBJ) $(EXECUTABLE)

$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

clean:
	$(RM) $(OBJ) $(EXECUTABLE)
