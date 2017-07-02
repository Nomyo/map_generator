CXX = g++
CXXFLAGS = -Wall -std=c++14 -pedantic -g
LDFLAGS = -lGLEW -lglfw3 -lGL -lm -ldl -lXinerama \
	-lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSDL2main -lSDL2 \
	-lglut -lSDL2_image -lGL -lGLU

DIR = src
SOURCE = $(DIR)/utils.cc $(DIR)/simplexnoise.cc $(DIR)/main.cc \
	 $(DIR)/camera.cc

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
