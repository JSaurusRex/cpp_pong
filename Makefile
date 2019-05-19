all: main.cpp
	g++ -o pong main.cpp -I./include -lglfw -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
