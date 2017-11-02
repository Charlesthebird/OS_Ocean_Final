#ifndef STB_IMAGE_IMPLEMENTATION
//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//std
#include <iostream>
//image reading/writing library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//Custom
#include "Shader.h"

using namespace std;
using namespace glm;

#endif // !libGL



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow * window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* window;


//starting camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float cameraPitch;
float cameraYaw = -90.0f;
float fov = 45.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

						// mouse input variables
bool firstMouse = true;
float lastX = 400, lastY = 300;



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	cameraYaw = glm::mod(cameraYaw + xoffset, 360.0f);
	cameraPitch += yoffset;

	if (cameraPitch > 89.0f)
		cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)
		cameraPitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	front.y = sin(glm::radians(cameraPitch));
	front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera movement
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 5;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 8); // antialiasing
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("vertex1.vs", "fragment1.fs");

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// build and compile our shader zprogram
	// ------------------------------------
	//Shader lightingShader("1.colors.vs", "1.colors.fs");
	//Shader lampShader("1.lamp.vs", "1.lamp.fs");






	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	/*
	float vertices[] = {
	// triangle Left
	-1.0f, -0.5f, 0.0f, // left
	0.0f, -0.5f, 0.0f, // right
	-.5f,  0.0f, 0.0f,
	// triangle Right
	0.0f, -0.5f, 0.0f, // left
	1.0f, -0.5f, 0.0f, // right
	.5f,  0.0f, 0.0f,
	// triangle Top
	-.5f,  0.0f, 0.0f, // left
	.5f,  0.0f, 0.0f, // right
	0.0f, 0.5f, 0.0f
	};
	//*/
	/*	float vertices[] = {
	// positions          // colors           // texture coords
	//front
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left
	//back
	0.5f,  0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f    // top left
	};*/
	float vertices[] = {
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
	unsigned int indices[] = {
		//front
		0, 1, 3,
		2, 1, 3,
		//back
		4, 5, 7,
		6, 5, 7,
		//top
		0, 3, 4,
		0, 7, 4,
		//bottom
		1, 2, 5,
		6, 5, 2,
		//left
		2, 3, 7,
		2, 6, 7,
		//right
		0, 1, 5,
		0, 4, 5
	};
	int vn = sizeof(vertices) / sizeof(float);
	int en = sizeof(indices) / sizeof(int);
	cout << vn << endl;


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// ATTRIBUTES ARE UNIQUE TO EACH VERTEX SHADER CALL
	// total size of the attributes are the number of indices of info are needed for each vertex
	// location 0 = position, position has a size of 3 (x,y,z), and an offset of 0 since it is at the first index. 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// location 1 = texture. texture has a size of 2 (u,v), and an offset of 3 * sizeof(float) for each attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);





	//// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	//unsigned int lightVAO;
	//glGenVertexArrays(1, &lightVAO);
	//glBindVertexArray(lightVAO);

	//// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; 
	//// the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//------------------------- textures --------------------------------------//



	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
											// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------

	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
					 // either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

	// ------------------- disable cursor -------------------- //
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//------------------------- while loop --------------------------------------//

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		processInput(window);

		// update vertices
		// -----
		float timeValue = glfwGetTime();
		ourShader.setFloat("atime", timeValue);
		/*

		float greenValue = (sin(20 * timeValue) / 4.0f) + 0.5f;
		for (int i = 5; i < vn; i += 8) {
		vertices[i] = greenValue;
		}

		*/

		mat4 view;
		/*
		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mouseState == GLFW_PRESS) {
		//cout << "mouse down" << endl;
		//vertices[0] = 1;
		//ourShader.set4f("addColor", .2,.05,.1,1);
		view = glm::mat4();
		view = glm::translate(view, glm::vec3(0.0f, 0.0f/*glfwGetTime() * -9.81f* /, -3.0f ));
		}
		else if(mouseState == GLFW_RELEASE) {
		//ourShader.set4f("addColor", 0,0,0,0);
		//vertices[0] = .5f;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f/*glfwGetTime() * -9.81f* /, -3.0f - (2 * glfwGetTime())));
		}
		*/



		//---------------------------transforms---------------------------------//
		/*
		vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		/*mat4 trans;
		trans = translate(trans, vec3(1.0f, 1.0f, 0.0f));
		vec = trans * vec;
		cout << vec.x << vec.y << vec.z << endl;
		mat4 trans;
		trans = rotate(trans, radians(90.0f), vec3(0.0, 0.0, 1.0));
		trans = scale(trans, vec3(0.5, 0.5, 0.5));* /
		mat4 trans;
		trans = rotate(trans, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
		trans = translate(trans, vec3(0.5f, -0.15f, 0.0f));
		//trans = rotate(trans, glm::sin((float)glfwGetTime()), vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		*/



		//------------------------- transformation matrices --------------------------------------//
		// The vertices are initially in the local space. They are then multiplied by the: 
		//		- Model matrix to get into world space, then the
		//		- View matrix to get into camera space, then the
		//		- Projection matrix to get into clip space 
		//			(can be orthographic or perspective, and frustrum culling/clipping occurs at this step)
		// Each transformation matrix has position, rotation, and scale values for the x, y, and z axes

		// model matrix is used to transform from local to world space
		//glm::mat4 model;
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		// view matrix is used to transform from world to view space
		//glm::mat4 view;
		// note that we're translating the scene in the reverse direction of where we want to move 
		// (since the z direction is positive towards the camera)
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f/*glfwGetTime() * -9.81f*/, -3.0f - (2 * glfwGetTime())));

		// projection matrix is used to transform from view to clip space
		glm::mat4 projection;
		//projection = glm::perspective(glm::radians(45.0f), (SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(fov), (SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 100.0f);

		//ourShader.set4m("model", model);
		//ourShader.set4m("view", view);
		ourShader.set4m("projection", projection);





		// render
		// ------
		glClearColor(.5, .6, .8, 1);//0.8f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// update the buffer data (for mesh animation)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// render objects
		ourShader.use();
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, vn, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, vn); // vn is the number of elements in the vertices array

		glBindVertexArray(VAO);
		float speed = 100.0f;
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);

			float angle = (20.0f * i);
			if (i % 3 == 0) angle += speed * glfwGetTime();
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.set4m("model", model);

			glDrawArrays(GL_TRIANGLES, 0, vn);
		}



		//---------------------------- camera --------------------------------------//

		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		//float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		ourShader.set4m("view", view);





		/*
		//------------------------- transformation matrices for object 2 --------------------------------------//
		// model matrix is used to transform from local to world space
		model = glm::mat4();
		model = glm::rotate(model, glm::radians(-75.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(.3f, .3f, 1));
		// view matrix is used to transform from world to view space
		view = glm::mat4();
		view = glm::translate(view, glm::vec3(0.0f, -.5f, -2.0f));
		// projection matrix is used to transform from view to clip space
		projection = glm::mat4();
		projection = glm::perspective(glm::radians(45.0f), (SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 100.0f);

		ourShader.set4m("model", model);
		ourShader.set4m("view", view);
		ourShader.set4m("projection", projection);

		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		//glDrawElements(GL_TRIANGLES, vn, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, vn); // vn is the number of elements in the vertices array
		*/






		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//*/


		// update frame data
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

