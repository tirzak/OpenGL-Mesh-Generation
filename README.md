
## Mesh Generation - Marching Cubes Algorithm
There are three sources (camera.cpp, write_ply.cpp, and L20.cpp). 
Overview:

The code has been divided into files according to their function. 

In L20.cpp, the marching_cubes generates the vertices using the lookuptables in TriTable.hpp. It also contains compute_normals function. The marching_cubes function pushes the values into an interleaved array
that has the format v_x,v_y,v_z,n_x,n_y,n_z. Interleaved array is being used for easy specification of layout in VBO.
drawCubeBorders() function is used to draw the cube.


In write_ply.cpp, the function WriteToPly writes the vertices,faces,and normals to the ply file. mesh1.ply contains the data of first scalar field and mesh2.ply contains the data for the second one

camera.cpp contains the code for camera controls. Click and drag to rotate, and press up arrow or down arrow to change radius. The position are calculated using spherical coordinates. To prevent gimball lock, phi angle is clamped

Shaders: 
The vertex and fragment shaders are in files CG3.vertexshader and CG3.fragmentshader respectively. Phong like shading is used to color the meshes

Cam Controls:
Up and Down arrow key to adjust radius. Use click and drag to rotate around.


Dependencies:
GLEW 
OpenGL 
GLFW 
GLM
C++11

Building:
Can be built using the Makefile.
Open the Makefile. If GLM headers are in your PATH environment variable, remove the -I/home/tirzak/Downloads/glm
Else, update the path in the -I/home/tirzak/Downloads/glm to point to the GLM headers location in your machine.
Following command can be used to build with Makefile: (Assuming you are in the directory with Makefile, and your platform is Linux):

make


To run:
Use the format function number(1 or 2) screenW(optional) screenH(optional)
Example:
Mesh1: 
./marching_cubes.out 1

Mesh2: 
./marching_cubes.out 2

With screenW and screenH
./marching_cubes.out 1 800 800
or,
./marching_cubes.out 2 800 800


References:
http://paulbourke.net/geometry/polygonise/