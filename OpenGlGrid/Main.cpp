#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Shaders.h"
#include<Soil/SOIL.h>
#include "assimp/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GoodMesh.h"


glm::vec3 cameraPos = glm::vec3(15.0f, 4.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(20.0f, -10.0f, -1.0f);

glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDown = glm::vec3(0.0f, -1.0f, 0.0f);



void key_callback(GLFWwindow*, int, int, int, int);
bool keys[1024];
void do_movement();
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
GLfloat lastX = 550, lastY = 450;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat yaw = -135.0f;
GLfloat pitch = -15.0f;
GLfloat fov = 45.0f;
int CellCount = 0;
bool show_demo_window = true;
bool show_another_window = false;

int main()
{
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(1100, 900, "Cubes", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Setup Dear ImGui context
	//stbi_set_flip_vertically_on_load(true);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, 1100, 900);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	/*glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(0.0f, 0.6f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;

	trans = glm::rotate(trans, 0.0f, glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));*/
	

	Shader ShaderProgram("ShVert.vert", "ShFrag3.frag");

	Shader ShaderProgram2("vertices.vert", "ShFrag3.frag");

	GoodModel ourModel;
	ourModel.LoadModel("C:/1Projects/OpenGlGrid/OpenGlGrid/Models/Isabella1.111.stl");

	GLfloat verticesForRect[] = {
		// Позиции          // Цвета             // Текстурные координаты
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
	};

	GLfloat VerticesForPiramid[] =
	{
		0.0f,	1,		0.0f,	0.5, 1,//Вершина пирамиды
		-0.5f,	0.0f, -0.5f,	0, 0,
		0.5f,	0.0f, -0.5f,	1, 0,

		0.5f,	0.0f,  0.5f,	1, 0,

		-0.5f,	0.0f,  0.5f,	0, 0,
	};

	GLuint indicesForPiramid[] =
	{
		1,0,2,
		2,0,3,
		3,0,4,// Upper triangle
		1,0,4,// Upper triangle
		1,2,4,
		2,4,3

	};

	GLfloat texCoords[] = {
	0.0f, 0.0f,  // Нижний левый угол 
	1.0f, 0.0f,  // Нижний правый угол
	0.5f, 1.0f   // Верхняя центральная сторона
	};

	GLfloat VerticesForCube[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 3, // Lower left triangle
		1, 2, 3, // Upper triangle

	};

	GLfloat firstTriangle[] = {
		//Вершины           //Цвета
		-0.9f, -0.9f, 0.0f, 1.0f, 0.0f, 0.0f,  // Left 
		-0.9f, -0.85f, 0.0f, 0.0f, 1.0f, 0.0f,  // Right
		-0.85f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f   // Top 
	};

	GLfloat secondTriangle[] = {
		 0.005f, 0.05f, 0.0f, -0.03f, 0.03f, 0.02f,  // Left
		 0.005f, 0.05f, 0.0f, 0.02f, -0.03f, 0.03f,  // Right
		 0.005f, 0.05f, 0.0f, 0.03f, 0.02f, -0.03f  // Top 
	};

	GLfloat VerticesForLines[363];
	GLuint IndicesForLines[470];

	int VerticesCount = 0;
	int indicesCount = 0;

	int currentIndicesPos = 0;
	bool endoflinemarker = false;
	bool newLine = true;
	for (float i = 0.0f; i <= 10; i=i+1)
	{
		for (float j = 0.0f; j <= 10; j=j+1)
		{

			VerticesForLines[VerticesCount] = j / 10.0f;//x
			VerticesCount++;
			VerticesForLines[VerticesCount] = 0.0f;		//y;
			VerticesCount++;
			VerticesForLines[VerticesCount] = i / 10.0f;	//z
			VerticesCount++;

			if (currentIndicesPos!=0 && fmod(currentIndicesPos - i, 10) == 0)
			{
				if (endoflinemarker) {
					endoflinemarker = false;
					newLine = true;
				}
				else {
					endoflinemarker = true;
				}
			}
			else {
				endoflinemarker = false;
			}

			if (currentIndicesPos + 11 <= 120) {
				IndicesForLines[indicesCount] = currentIndicesPos;
				indicesCount++;
				IndicesForLines[indicesCount] = currentIndicesPos + 11;
				indicesCount++;
			}
			
			if (currentIndicesPos + 1 <= 120 && !endoflinemarker)
			{
				IndicesForLines[indicesCount] = currentIndicesPos;
				indicesCount++;
				IndicesForLines[indicesCount] = currentIndicesPos + 1;
				indicesCount++;
			}
			
			currentIndicesPos++;

		}
	}

	GLfloat VerticesForCoordinates[12]{
		0,4,0,
		0,-0.1,4,
		4,-0.1,0,
		0,-0.1,0
	};

	GLuint IndicesForCoordinates[6]{
	3,0,
	3,1,
	3,2
	};


	const int overall = 1;

	GLuint VBOs, VAOs, EBO, VBOsLines, 
		VAOsLines, VBOsCoordinates, VAOsCoordinates;

	glGenVertexArrays(overall, &VAOs);
	glGenBuffers(overall, &VBOs);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAOs);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesForPiramid), VerticesForPiramid, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForPiramid), indicesForPiramid, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Сетка--
	glGenVertexArrays(overall, &VAOsLines);
	glGenBuffers(overall, &VBOsLines);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAOsLines);
	glBindBuffer(GL_ARRAY_BUFFER, VBOsLines);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesForLines), VerticesForLines, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesForLines), IndicesForLines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//--Сетка

	//Координаты мирового пространства
	glGenVertexArrays(overall, &VAOsCoordinates);
	glGenBuffers(overall, &VBOsCoordinates);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAOsCoordinates);
	glBindBuffer(GL_ARRAY_BUFFER, VBOsCoordinates);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesForCoordinates), VerticesForCoordinates, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesForCoordinates), IndicesForCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Координаты мирового пространства
	glm::vec3 cubePositions[] = {
			glm::vec3(5.0f,  0.2f,   7.0f)
	};

	
	glm::mat4 view;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::string FPS;
	double prevTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	glEnable(GL_DEPTH_TEST);

	//Угол камеры
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		do_movement();
		glLineWidth(1);
	
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		glUseProgram(ShaderProgram.ID);

		glm::mat4 projection;
		projection = glm::perspective(fov, 800.0f / 800.0f, 0.1f, 10000.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//glm::mat4 trans;
		glClearColor(0.1f, 0.23f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model;
		GLint modelLoc = glGetUniformLocation(ShaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint viewLoc = glGetUniformLocation(ShaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLint projectionLoc = glGetUniformLocation(ShaderProgram.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Рисуем сетку
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		glm::vec3 PositionInWorld= glm::vec3(x, y, z);
		glm::vec4 color = glm::vec4(0.7f, 0.9f, 0.8f, 1.0f);
		ShaderProgram.setVec4("vertexColor", color);
		glBindVertexArray(VAOsLines);
		int j = 0;
		while (j <= 10)
		{
			int i = 0;
		
			while (i <= 10)
			{
				glm::mat4 model;
				PositionInWorld = glm::vec3(x, y, z);
				model = glm::translate(model, PositionInWorld);
				modelLoc = glGetUniformLocation(ShaderProgram.ID, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawElements(GL_LINES, sizeof(IndicesForLines), GL_UNSIGNED_INT, 0);
				std::cout << "x="; std::cout << PositionInWorld.x << std::endl;
				std::cout << "z="; std::cout << PositionInWorld.z << std::endl;
				x = x + 1;
				i++;
			}

			x = 0;
			z = z + 1.0f;
			j++;
		}

		//Пирамиды
		color = glm::vec4(0.2f, 0.f, 0.5f, 1.0f);
		ShaderProgram.setVec4("vertexColor", color);
		glBindVertexArray(VAOs);
		for (GLuint i = 0; i <1; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
		
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		}
		glBindVertexArray(0);

		//Мировые координаты
		color = glm::vec4(1.0f, 0.0f, 0.3f, 1.0f);
		ShaderProgram.setVec4("vertexColor", color);
		glBindVertexArray(VAOsCoordinates);
		
		PositionInWorld = glm::vec3(0, 0, 0);
		glm::mat4 model1;
		model1 = glm::translate(model1, PositionInWorld);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glDrawElements(GL_LINES, sizeof(IndicesForCoordinates), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glUseProgram(ShaderProgram2.ID);

		glm::mat4 trans;
		model = glm::rotate(model, 270.0f, glm::vec3(1, 0, 0));
		//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0, 0, 1));
		projection = projection * trans;
		modelLoc = glGetUniformLocation(ShaderProgram2.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		viewLoc = glGetUniformLocation(ShaderProgram2.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		projectionLoc = glGetUniformLocation(ShaderProgram2.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		

		//Рисуем линии по краям треугольников
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		color = glm::vec4(0.5f, 0.1f, 0.1f, 1.0f);
		ShaderProgram2.setVec4("vertexColor", color);
		glm::mat4 model2 = glm::mat4(1.0f);
		ShaderProgram2.setMat4("model2", model2);
		ourModel.Draw();

		//заполняем треугольники цветом
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		color = glm::vec4(0.8f, 0.7f, 0.1f, 1.0f);
		ShaderProgram2.setVec4("vertexColor", color);
		glLineWidth(1);
		glm::mat4 model23 = glm::mat4(1.0f);
		ShaderProgram2.setMat4("model23", model23);
		ourModel.Draw();


		static float f = 0.0f;
		static int counter = 0;

		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glDeleteVertexArrays(2, &VAOs);
	glDeleteBuffers(2, &VBOs);

	glDeleteVertexArrays(2, &VAOsLines);
	glDeleteBuffers(2, &VBOsLines);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
	// Delete window before ending the program

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	GLfloat cameraSpeed = 0.05f;
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
	if (key == GLFW_KEY_W)
		cameraPos += cameraSpeed * cameraFront;
	if (key == GLFW_KEY_S)
		cameraPos -= cameraSpeed * cameraFront;
	if (key == GLFW_KEY_A)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_D)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_SPACE)
		cameraPos += glm::normalize(cameraUp) * cameraSpeed;
	if (key == GLFW_KEY_C)
		cameraPos += glm::normalize(cameraDown) * cameraSpeed;
	if (key == GLFW_KEY_BACKSPACE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (key == GLFW_KEY_C) {
		if (keys[key] != true) {
			CellCount++;
		}
		
	}
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // эта переменная была проинициализирована значением true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void do_movement()
{
	GLfloat cameraSpeed = 10.0f * deltaTime;
	// Camera controls
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_SPACE])
		cameraPos += glm::normalize( cameraUp) * cameraSpeed;
	if (keys[GLFW_KEY_C])
		cameraPos += glm::normalize(cameraDown) * cameraSpeed;
}
