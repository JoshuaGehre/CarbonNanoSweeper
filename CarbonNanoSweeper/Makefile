# Not yet functional


LIBS = -L./GLFW/lib-linux-x86_64/

INCLUDES = -I./GLFW/include/ -I./libs/
# -I/usr/include/G.L/
# -I./glad/include

CPP=g++
CPP_OPTS=-Wall -Wextra --std=c++11 -O2 -g

HEADERS = Camera.h CNS.h DTGlobal.h Keys.h Loop.h Perspective.h RenderData.h Texture.h

OBJECTS = Main.o Camera.o CNS.o Keys.o Loop.o Perspective.o RenderData.o Texture.o

GLFW_OPT = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

.PHONY: clean

all: $(OBJECTS) 
	$(CPP) $(CPP_OPTS) -o CarbonNanoSweeper $(OBJECTS) $(LIBS) $(GLFW_OPT) 

Main.o: Main.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

Camera.o: Camera.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

CNS.o: CNS.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

Keys.o: Keys.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

Loop.o: Loop.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

Perspective.o: Perspective.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

RenderData.o: RenderData.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

Texture.o: Texture.cpp $(HEADERS)
	$(CPP) $(CPP_OPTS) -c $< $(GLFW_OPT) -o $@ $(INCLUDES)

clean:
	rm $(OBJECTS)
