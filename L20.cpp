// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <functional>
#include <GL/glew.h>
#include "TriTable.hpp"
// Include GLFW
#include <GLFW/glfw3.h>
#include "shader.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include <iostream>
#include <vector>
#include <array>
#include "write_ply.h"
#include "camera.h"
std::vector<float> interleaved;
long estimatedVertexCount = 6466600;
long cntOfVertex=0;
bool mousePressed = false;
double lastX, lastY;
float myCamerapitch = 0.0f, myCamerayaw = -90.0f;
float radius = 20.0f;
float cameraSpeed = 0.4f;
GLuint VBO;
GLuint VAO;
GLuint marchingCubesShaderProgramID;
glm::vec3 cameraPos = glm::vec3(5.0f, 5.0f, 5.0f);
glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);


GLFWwindow *windowInMain;

bool flag = false;

// Camera camera = Camera(eye, center, up, cameraYaw, cameraPitch, cameraSpeed, rotateDeg);
Camera camera(cameraPos, target, worldUp);
void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
	if (mousePressed)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		float speedOfDrag = 0.1f;
		xoffset *= speedOfDrag;
		yoffset *= speedOfDrag;

		camera.processRotation(xoffset, yoffset);

		lastX = xpos;
		lastY = ypos;
	}
}
void handleInput(GLFWwindow *window, glm::vec3 &lpos, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.processRadiusUpdate(-1 * cameraSpeed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.processRadiusUpdate(cameraSpeed * deltaTime);
	}

}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mousePressed = true;
			glfwGetCursorPos(window, &lastX, &lastY);
		}
		else if (action == GLFW_RELEASE)
		{
			mousePressed = false;
		}
	}
}
void drawCubeBorders(glm::vec3 max, glm::vec3 min)
{
	
	glm::vec3 vertices[8] = {
		glm::vec3(min.x, min.y, min.z), 
		glm::vec3(max.x, min.y, min.z), 
		glm::vec3(max.x, min.y, max.z), 
		glm::vec3(min.x, min.y, max.z), 
		glm::vec3(min.x, max.y, min.z), 
		glm::vec3(max.x, max.y, min.z), 
		glm::vec3(max.x, max.y, max.z), 
		glm::vec3(min.x, max.y, max.z)	
	};
	
	GLint edges[24] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7};
	glColor3f(1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLineWidth(2.0f);

	glBegin(GL_LINES);
	for (int i = 0; i < 12; i++)
	{
		if (edges[i * 2] == 0 && edges[i * 2 + 1] == 1)
			continue;
		if (edges[i * 2] == 3 && edges[i * 2 + 1] == 0)
			continue;
		if (edges[i * 2] == 0 && edges[i * 2 + 1] == 4)
			continue;
		glVertex3f(vertices[edges[i * 2]].x, vertices[edges[i * 2]].y, vertices[edges[i * 2]].z);
		glVertex3f(vertices[edges[i * 2 + 1]].x, vertices[edges[i * 2 + 1]].y, vertices[edges[i * 2 + 1]].z);
	}
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
	glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);

	// Little line on top
	//  glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	//  glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z+0.1);

	// glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	// glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z-0.1);
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
	glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

	// glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	// glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z+0.1);
	// glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	// glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z-0.1);

	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
	glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);

	// glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
	// glVertex3f(vertices[4].x+0.1, vertices[4].y, vertices[4].z);
	// glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
	// glVertex3f(vertices[4].x-0.1, vertices[4].y, vertices[4].z);

	glEnd();
	glPopMatrix();
}




float f(float x, float y, float z)
{
	float num= y - sin(x) * cos(z);
	return num;
}
float f2(float x, float y, float z)
{
	
	float num = (x * x) - (y * y) - (z * z) - z;
	return num;
}

constexpr int TOP_LEFT = 1;
constexpr int TOP_RIGHT = 2;
constexpr int BOTTOM_RIGHT = 4;
constexpr int BOTTOM_LEFT = 8;

void setMyShaderUniforms(glm::mat4 &model, glm::mat4 &VIEW,glm::mat4 &Projection, glm::vec3 &lightpos){
		glm::mat4 MVP = Projection * VIEW * model;
		glm::vec3 modelColor = glm::vec3(0.384f, 0.839f, 0.843f);

		glUniformMatrix4fv(glGetUniformLocation(marchingCubesShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(marchingCubesShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(VIEW));

		glUniformMatrix4fv(glGetUniformLocation(marchingCubesShaderProgramID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform3fv(glGetUniformLocation(marchingCubesShaderProgramID, "modelColor"), 1, glm::value_ptr(modelColor));
		glUniform3fv(glGetUniformLocation(marchingCubesShaderProgramID, "lightPos"), 1, glm::value_ptr(lightpos));
}
void updateBuffer(std::vector<float> &vertices_and_normals){

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glBufferSubData(GL_ARRAY_BUFFER, interleaved.size() * sizeof(float), vertices_and_normals.size() * sizeof(float), vertices_and_normals.data());
		
		interleaved.insert(interleaved.end(), vertices_and_normals.begin(), vertices_and_normals.end());
		
		glDrawArrays(GL_TRIANGLES, 0, interleaved.size() / 6);

		vertices_and_normals.erase(vertices_and_normals.begin(), vertices_and_normals.end());

		int isd = 0;
		// while(isd!=INT_MAX-1){
		// 	isd++;
		// }
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwSwapBuffers(windowInMain);
		glfwPollEvents();
}
std::vector<float> compute_normals(const std::vector<float> &marchingVerts)
{
	std::vector<float> normals;

	for (int i = 0; i < (int)marchingVerts.size(); i += 9)
	{
		glm::vec3 v1(marchingVerts[i], marchingVerts[i + 1], marchingVerts[i + 2]);
		glm::vec3 v2(marchingVerts[i + 3], marchingVerts[i + 4], marchingVerts[i + 5]);
		glm::vec3 v3(marchingVerts[i + 6], marchingVerts[i + 7], marchingVerts[i + 8]);

		glm::vec3 edge1 = v2 - v1;
		glm::vec3 edge2 = v3 - v1;

		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		for(int i=0 ; i<3;++i){
			for (int j = 0; j < 3; ++j)
			{
				normals.push_back(normal[j]);
			}
		}
	

	}

	return normals;
}
void marching_cubes(std::function<float(float, float, float)> f, float isovalue, float min, float max, float stepsize)
{
	std::vector<float> vertices_and_normals;
	for (float x = min; x < max; x += stepsize)
	{
		for (float y = min; y < max; y += stepsize)
		{
			for (float z = min; z < max; z += stepsize)

			{
				float cube[8];

				cube[0] = f(x, y, z);
				cube[1] = f(x + stepsize, y, z);
				cube[2] = f(x + stepsize, y, z + stepsize);

				cube[3] = f(x, y, z + stepsize);
				cube[4] = f(x, y + stepsize, z);
				cube[5] = f(x + stepsize, y + stepsize, z);
				cube[6] = f(x + stepsize, y + stepsize, z + stepsize);

				cube[7] = f(x, y + stepsize, z + stepsize);

				int cubeIndex = 0;
				const float EPSILON = 1e-6f;

				for (int i = 0; i < 8; ++i)
				{
					if (cube[i] < isovalue)
					{
						cubeIndex |= (1 << i);
					}
				}
				int *vertsPointer = marching_cubes_lut[cubeIndex];

				for (int i = 0; i < 16; i += 3)
				{
					if (vertsPointer[i] == -1)
					{
						break;
					}

					glm::vec3 triangleVertices[3];
					for (int j = 0; j < 3; ++j)
					{
						int vertexIndex = vertsPointer[i + j];
						triangleVertices[j] = {x + stepsize * vertTable2[vertexIndex][0], y + stepsize * vertTable2[vertexIndex][1], z + stepsize * vertTable2[vertexIndex][2]};
					}

					if (triangleVertices[0] == triangleVertices[1] || triangleVertices[0] == triangleVertices[2] || triangleVertices[1] == triangleVertices[2])
					{
						std::cout << "Found Degen" << std::endl;
						continue;
					}
			
					glm::vec3 edge1 = triangleVertices[1] - triangleVertices[0];
					glm::vec3 edge2 = triangleVertices[2] - triangleVertices[0];

				
					glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

		
					for (int j = 0; j < 3; ++j)
					{
						cntOfVertex+=6;
						vertices_and_normals.push_back(triangleVertices[j].x);
						vertices_and_normals.push_back(triangleVertices[j].y);
						vertices_and_normals.push_back(triangleVertices[j].z);

						vertices_and_normals.push_back(normal.x);
						vertices_and_normals.push_back(normal.y);
						vertices_and_normals.push_back(normal.z);
					}
				}
			}
		}
		updateBuffer(vertices_and_normals);
	}
	flag=true;
	std::cout << interleaved.size() << std::endl;
	
}



//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{

	///////////////////////////////////////////////////////
	float screenW = 1366;
	float screenH = 768;
	int chosenFunc = 1;
	if (argc > 1)
	{
		chosenFunc = atoi(argv[1]);
	}
	if (argc > 2)
	{
		screenW = atoi(argv[2]);
	}
	if (argc > 3)
	{
		screenH = atoi(argv[3]);
	}
	///////////////////////////////////////////////////////

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	windowInMain = glfwCreateWindow(screenW, screenH, "Assignment 5", NULL, NULL);
	if (windowInMain == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(windowInMain);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(windowInMain, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Projection = glm::mat4(1.0f);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	// Projection = glm::mat4(1.0f);
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
	glLoadMatrixf(glm::value_ptr(Projection));

	camera.updateCoords();

	glm::mat4 VIEW = camera.getViewMat();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(VIEW));
	// glm::vec3 lightpos(15.0f, 15.0f, 15.0f);

	glm::vec3 lightpos(96, 216, 107);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float isoval = 0;
	float isoval2 = -1.5;
	float stepsize = 0.05;

	float xmin = -5;
	float xmax = 5;
	glDisable(GL_CULL_FACE);


		// marchingVerts = boxyMarchingCubes(f, isoval2, xmin, xmax, stepsize);
		// marchingVerts = marchingCubes2Interp(f, isoval, xmin, xmax, stepsize);
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	marchingCubesShaderProgramID = LoadShaders("CG3.vertexshader", "CG3.fragmentshader");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, estimatedVertexCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLenum error;
	// Axes ax(target, glm::vec3(4.0f, 4.0f, 4.0f));

	while ((error = glGetError()) != GL_NO_ERROR)
	{

		std::cerr << "OpenGL error " << error << std::endl;
	}
	int cnt = 0;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	glfwSetMouseButtonCallback(windowInMain, mouseButtonCallback);
	glfwSetCursorPosCallback(windowInMain, cursorPositionCallback);

	glm::mat4 model = glm::mat4(1.0f);
	do
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ax.draw();
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		handleInput(windowInMain, lightpos, deltaTime);
		drawCubeBorders({xmax, xmax, xmax}, {xmin, xmin, xmin});

		
		glUseProgram(marchingCubesShaderProgramID);
		VIEW = camera.getViewMat();
		setMyShaderUniforms(model, VIEW, Projection,lightpos);
		cnt++;
		if ((error = glGetError()) != GL_NO_ERROR)
		{

			std::cerr << "OpenGL Load error " << cnt << " " << error << std::endl;
		}
		
		if (interleaved.empty())
		{
			if(chosenFunc==2){
				marching_cubes(f2, isoval2, xmin, xmax, stepsize);

			}
			else {
				marching_cubes(f, isoval, xmin, xmax, stepsize);

			}
		}
		else if (flag)
		{
			glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES, 0, interleaved.size() / 6);

			glBindVertexArray(0);
		}

		glUseProgram(0);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(VIEW));
		// Swap buffers
		glfwSwapBuffers(windowInMain);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(windowInMain, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(windowInMain) == 0);

   
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(marchingCubesShaderProgramID);
	std::string fileName = "mesh"+std::to_string(chosenFunc)+".ply";

	WriteToPly(interleaved, fileName);
	glfwTerminate();
	return 0;
}
