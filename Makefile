all: main.cpp
	g++ -o pong main.cpp -O3 -I./include -lglfw -lGL -lGLEW -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
