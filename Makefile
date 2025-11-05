# COMMAND TO USE:
# make all
# ./app/outputs/particle_sim

#OBJS specifies which files to compile as part of the project
OBJS = app/src/*.cpp app/src/Helpers/*.cpp app/src/Colliders/*.cpp app/src/Colliders/Circle/*.cpp
BUILD_DIR = mkdir -p app/outputs

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -O2 run in parallel (Optimisation)
COMPILER_FLAGS = -w -O2

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -pthread

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = app/outputs/particle_sim

#This is the target that compiles our executable
all : $(OBJS) 
	$(BUILD_DIR)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)