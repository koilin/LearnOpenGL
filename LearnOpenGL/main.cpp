#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>

#include <learnopengl/shader_m.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow * window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float fFac = 0.2f;

int main()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float vertices1[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to crete GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	Shader cubeShader("cube.vert", "cube.frag");

	Shader lightShader("light.vert", "light.frag");

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/*
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data1 = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture!" << std::endl;
	}
	stbi_image_free(data1);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture!" << std::endl;
	}
	stbi_image_free(data2);
	*/

	cubeShader.use();
	//ourShader.setInt("ourTexture", 0);
	//ourShader.setInt("faceTexture", 1);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_2D, texture1);
// 		glActiveTexture(GL_TEXTURE1);
// 		glBindTexture(GL_TEXTURE_2D, texture2);
		cubeShader.use();

		glm::mat4 matView = glm::mat4(1.0);
		glm::mat4 matProj = glm::mat4(1.0);
		glm::mat4 matModel = glm::mat4(1.0);

// 		GLfloat fRadius = 10.0f;
// 		GLfloat fCamX = sin(glfwGetTime()) * fRadius;
// 		GLfloat fCamZ = cos(glfwGetTime()) * fRadius;
		matView = glm::lookAt(glm::vec3(10.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		matProj = glm::perspectiveFov(glm::radians(45.0f), (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.1f, 100.0f);
		matModel = glm::translate(matModel, cubePositions[0]);
		cubeShader.setMat4("matModel", matModel);
		cubeShader.setMat4("matView", matView);
		cubeShader.setMat4("matProj", matProj);
		cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

// 		for (uint i = 0; i < 10; i++)
// 		{
// 			glm::mat4 matModel = glm::mat4(1.0);
// 
// 
// 			matModel = glm::translate(matModel, cubePositions[i]);
// 			float fAngle = 20.0f * i;
// 			matModel = glm::rotate(matModel, glm::radians(fAngle), glm::vec3(1.0f, 0.3f, 0.5f));
// 			cubeShader.setMat4("matModel", matModel);
// 			cubeShader.setMat4("matView", matView);
// 			cubeShader.setMat4("matProj", matProj);
// 
// 			glBindVertexArray(VAOCube);
// 			glDrawArrays(GL_TRIANGLES, 0, 36);
// 		}


		lightShader.use();
		glm::mat4 matLightModel = matModel;
		matLightModel = glm::translate(matLightModel, lightPos);
		matLightModel = glm::scale(matLightModel, glm::vec3(0.2f));
		lightShader.setMat4("matModel", matLightModel);
		lightShader.setMat4("matView", matView);
		lightShader.setMat4("matProj", matProj);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (fFac > 1.0)
		{
			fFac = 1.0;
		}
		else
		{
			fFac += 0.01f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (fFac < 0.0)
		{
			fFac = 0.0;
		}
		else
		{
			fFac -= 0.01f;
		}

	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}