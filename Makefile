CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17
LDFLAGS = -Wno-deprecated-declarations -std=c++17
LBLIBS = 

OS := $(shell uname)
ifeq ($(OS),Darwin)
	LBLIBS += -framework OpenGL -framework GLUT -framework Cocoa 
else
	LBLIBS += -lGL -lGLU -lglut
endif

LIBSFOLDER = headers
SRC ?= *.cpp
OBJ = $(SRC:.cc=.o)
EXEC = run.app
CPATH = /headers

all: clean $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -I /headers -I /code -o $@ $(OBJ) $(LBLIBS) 

clean:
	rm -rf *.o $(EXEC)

run: all
	./$(EXEC) $(FILE)