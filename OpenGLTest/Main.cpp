#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Ocean.h"

#include "SocketConnection.h"

#include <iostream>
#include <thread>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int main();
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0, .6f, -1.2f), glm::vec3(0, 1.0f, 0), 90, -10);//(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;

// my model
Model m;
glm::vec3 myPos = glm::vec3(0, 0.0f, 0);
glm::vec3 myFront = glm::vec3(0, 0, 1.0f);
glm::vec3 myRight = glm::vec3(-1.0f, 0, 0);
float myVelocity = 0.0f;

// other models
vector<Model> models;
vector<float[3]> modelPos, modelRot;


float* vertices;


// ------------------------ noise used to make ocean in ocean.vs
glm::vec2 random2(glm::vec2 st) {
	st = glm::vec2(glm::dot(st, glm::vec2(127.1f, 311.7f)),
		glm::dot(st, glm::vec2(269.5f, 183.3f)));
	return -1.0f + 2.0f*glm::fract(sin(st)*43758.5453123f);
}

// Value Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/lsf3WH
float noise(glm::vec2 st) {
	glm::vec2 i = floor(st);
	glm::vec2 f = fract(st);

	glm::vec2 u = f*f*(3.0f - 2.0f*f);

	return glm::mix(glm::mix(glm::dot(random2(i + glm::vec2(0.0, 0.0)), f - glm::vec2(0.0, 0.0)),
		glm::dot(random2(i + glm::vec2(1.0, 0.0)), f - glm::vec2(1.0, 0.0)), u.x),
		glm::mix(glm::dot(random2(i + glm::vec2(0.0, 1.0)), f - glm::vec2(0.0, 1.0)),
			glm::dot(random2(i + glm::vec2(1.0, 1.0)), f - glm::vec2(1.0, 1.0)), u.x), u.y);
}

float getOceanYVal(float x, float z, float waveHeight, float waveSpeed) {
	float valueNoiseAmount = .2;
	float rollingWaveAmount = 1;
	float vnSlow = waveSpeed * .5;
	float vnHeight = waveHeight * 10;

	return (noise(glm::vec2((x + currentFrame * vnSlow) * vnHeight, (z + currentFrame * vnSlow) * vnHeight))) * valueNoiseAmount +
		((waveHeight * glm::sin(waveSpeed * (x - currentFrame))) -
		(waveHeight * glm::cos(waveSpeed * (z - currentFrame)))) * rollingWaveAmount;
}



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader lightShader("1.lamp.vs", "1.lamp.fs");

	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

	Shader oceanShader("ocean.vs", "ocean.fs");

	glm::vec3 lightPos = glm::vec3(1000, 1000, 1000);


	// Create Ocean
	// -----------
	Ocean ocean = Ocean();


	// Load This Player's Model
	// -----------
	Model ourModel("boat.obj/boat.obj"); // -------------------------- make parallel possibly

	//uniform vec3 objectColor;
	//uniform vec3 lightColor;
	//uniform vec3 lightPos;
	//uniform vec3 viewPos;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// make a new socket connection in a new thread 
	// (this will either connect to a peer or start listening for peers)
	// -----------
	SocketConnection s = SocketConnection("localhost");
	thread t1(&SocketConnection::searchForPeers, &s);
	
	
	
	
	
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(.4f, .7f, .9f, 1.0f);// (0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glm::mat4 projection, view, model;




		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model

		float modelX = myPos.x;//0.0f;
		float modelZ = myPos.z;//0.0f;
		float modelY = myPos.y;
		// raise the boat to the ocean level
		modelY += getOceanYVal(modelX, modelZ, ocean.waveHeight, ocean.waveSpeed);
		
		model = glm::translate(model, glm::vec3(modelX, modelY, modelZ)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);



		lightShader.use();


		// view/projection transformations
		projection = glm::mat4();
		view = glm::mat4();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		model = glm::mat4();
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		model = glm::translate(model, lightPos);
		lightShader.setMat4("model", model);
		//ourModel.Draw(lightShader);

		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setVec3("objectColor", glm::vec3(.5, .3, .8));
		ourShader.setVec3("lightColor", glm::vec3(.9, .9, 1));
		ourShader.setVec3("lightPos", lightPos);


		// update and draw ocean
		//ocean.Update();

		oceanShader.use();

		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::mat4();
		// center the ocean in the world view
		//model = glm::translate(model, glm::vec3(-ocean.width / 2.0f, ocean.height, -ocean.width / 2.0f));
		// center the ocean underneath the player
		model = glm::translate(model, glm::vec3(myPos.x, ocean.height, myPos.z));
		oceanShader.setMat4("projection", projection);
		oceanShader.setMat4("view", view);
		oceanShader.setMat4("model", model);
		oceanShader.setFloat("aTime", currentFrame);
		oceanShader.setFloat("waveHeight", ocean.waveHeight);
		oceanShader.setFloat("waveSpeed", ocean.waveSpeed);

		ocean.Draw(oceanShader);


		// update my ship model

		// update the other ship models

		// update the shared time

		// send variables to connected peers




		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	t1.join();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		glfwTerminate();
		//glfwDestroyWindow(window);
	}

	myVelocity = camera.MovementSpeed * deltaTime;

	// speedup
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(SPEEDUP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.ProcessKeyboard(SLOWDOWN, deltaTime);
	if (camera.Speedup)
		myVelocity *= camera.shiftSpeedup;

	// movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
		myPos += myFront * myVelocity;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		myPos -= myFront * myVelocity;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
		myPos -= myRight * myVelocity;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
		myPos += myRight * myVelocity;
	}
/*
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}