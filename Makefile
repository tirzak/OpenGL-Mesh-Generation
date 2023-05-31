
all: tess

tess:
	g++ camera.cpp write_ply.cpp L20.cpp -std=c++11 -lglfw -lGLEW -lOpenGL -I/home/tirzak/Downloads/glm -o marching_cubes.out

clean:
	rm -f marching_cubes.out
