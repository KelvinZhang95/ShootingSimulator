#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "shader.h"
#include "Camera.h"
#include "Model.h"
#include "Gameobject.h"

#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
using namespace std;
#define PI 3.1415926f
glm::mat4 TRSum;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void creatGameobject();
void processInput(GLFWwindow *window);
void updateLogic();
void updateKinematics();
void updateChildPosRot();
//void setTexture(unsigned int *texture, string  filepath, bool ispng);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

float speed = 2.0f;
bool isSpeed = false;

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

glm::vec3 lightPos(8.0f, 6.0f, 9.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 11.0f));
vector<GameObject> gameobjects;

double px, py, pz, rx, ry, rz;

bool isPerspective  = true;
int main()
{
	
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);//anti-aliasing

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PlayGround", NULL, NULL);
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
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	//Shader ourShader("shader/shader.vs", "shader/shader.fs");
	Shader lightingShader("shader/lightshader.vs", "shader/lightshader.fs");
	Shader simpleDepthShader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");

	// Configure depth map FBO
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	lightingShader.use();
	lightingShader.setInt("diffuseTexture", 0);
	lightingShader.setInt("shadowMap", 1);

	//gameobjects.push_back(camera);
	creatGameobject();
	gameobjects[1].setParent(camera);
	//glm::vec3 lightPos(1.0f, 6.0f, 1.0f);

	// load textures (we now use a utility function to keep the code more organized)
	// render loop

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//cout << "fps : " << 1 / deltaTime << endl;

		// input
		processInput(window);

		updateLogic();
		// 1. Render depth of scene to texture (from ligth's perspective)
		// - Get light projection/view matrix.
		//glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 30.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// - now render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);


		glm::mat4 model;
		for (int i = 0; i < gameobjects.size(); i++) {
			model = glm::mat4();
			glm::mat4 trans = glm::translate(glm::mat4(), gameobjects[i].position);
			glm::mat4 rot = glm::eulerAngleXYZ(gameobjects[i].rotation.x, gameobjects[i].rotation.y, gameobjects[i].rotation.z);
			glm::mat4 sca = glm::scale(model, gameobjects[i].scale);
			model = trans * rot * sca;
			gameobjects[i].model.Draw(simpleDepthShader, false);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// render as normal
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		
		lightingShader.use();

		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 view;
		//view = glm::lookAt(camera.position, camera.position + camera.Front, camera.Up);
		//lightingShader.setMat4("view", view);
		//lightingShader.setMat4("projection", projection);

		lightingShader.setVec3("viewPos", camera.position);
		lightingShader.setVec3("lightPos", lightPos);

		lightingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 projection;
		if (isPerspective) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}
		else {
			projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		}

		glm::mat4 view;
		view = glm::lookAt(camera.position, camera.position + camera.Front, camera.Up);

		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		for (int i = 0; i < gameobjects.size(); i++) {
			model = glm::mat4();
			glm::mat4 trans = glm::translate(glm::mat4(), gameobjects[i].position);
			glm::mat4 rot = glm::eulerAngleXYZ(gameobjects[i].rotation.x, gameobjects[i].rotation.y, gameobjects[i].rotation.z);
			glm::mat4 sca = glm::scale(model, gameobjects[i].scale);
			model = trans * rot * sca;
			lightingShader.setMat4("model", model);
			gameobjects[i].model.Draw(lightingShader,true);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	bool hasMoved = false;
	glm::vec3 tryPosition;
	float cameraSpeed = !isSpeed ? speed * deltaTime : speed * 4 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		hasMoved = true;
		tryPosition = camera.position + cameraSpeed * camera.Front;
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		hasMoved = true;
		tryPosition = camera.position - cameraSpeed * camera.Front;

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		hasMoved = true;
		tryPosition = camera.position - glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		hasMoved = true;
		tryPosition = camera.position + glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		gameobjects[0].position = gameobjects[0].position + glm::vec3(0.1, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		gameobjects[0].setRotation(gameobjects[0].rotation + glm::vec3(1 * PI / 180, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		gameobjects[0].setRotation(gameobjects[0].rotation + glm::vec3(0, 1 * PI / 180, 0));

	}
	if (hasMoved) {
		for (int i = 0; i < gameobjects.size(); i++) {
			if (gameobjects[i].isActive) {
				if (gameobjects[i].model.collider.containPoint(tryPosition)) {
					cout << "in collider!" << endl;
					return;
				}
			}
		}
		camera.position = tryPosition;
	}
	
	cout << "camera.rotation" << camera.rotation.x / PI * 180 << ", " << camera.rotation.y / PI * 180 << ", " << camera.rotation.z / PI * 180 <<endl;
}
void updateChildPosRot()
{
	camera.updatePosRotFromParent();
	for (int i = 0; i < gameobjects.size(); i++) {
		gameobjects[i].updatePosRotFromParent();
	}
}
void updateKinematics()
{
	for (int i = 0; i < gameobjects.size(); i++) {
		if (gameobjects[i].parent == NULL) {//û�и��׵�ʱ��

		}
	}
}
//update logic 
void updateLogic() {
	updateKinematics();
	updateChildPosRot();
}





// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

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

	float sensitivity = 0.05f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera.Yaw += xoffset;
	camera.Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (camera.Pitch > 89.0f)
		camera.Pitch = 89.0f;
	if (camera.Pitch < -89.0f)
		camera.Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
	front.y = sin(glm::radians(camera.Pitch));
	front.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
	camera.Front = glm::normalize(front);
	camera.updateRotation();
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (camera.Zoom >= 1.0f && camera.Zoom <= 45.0f)
		camera.Zoom -= yoffset;
	if (camera.Zoom <= 1.0f)
		camera.Zoom = 1.0f;
	if (camera.Zoom >= 45.0f)
		camera.Zoom = 45.0f;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) 
		isSpeed = true;
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) 
		isSpeed = false;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		isPerspective = !isPerspective;
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		gameobjects[0].position = camera.position;
	}

		//creatGameobject();
}
void creatGameobject() {
	string path1 = "resources/model/dragon/dragon.obj";
	string path2 = "resources/model/nanosuit/nanosuit.obj";
	//string path3 = "resources/model/scene/scene.obj";
	string path4 = "resources/model/M4CQB/M4CQB.FBX";
	string path5 = "resources/model/dao/dao.fbx";

	//Gameobject dragon(path1, glm::vec3(4.0218, 10.5945, -0.371), glm::vec3(0.0020f, 0.0020f, 0.0020f));
	//dragon.isActive = false;
	//gameobjects.push_back(dragon);

	GameObject man(path2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 0, 0));
	//man.isActive = false;
	gameobjects.push_back(man);

	//Gameobject scene(path3, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3, 3, 3));
	//scene.isActive = false;
	//gameobjects.push_back(scene);

	GameObject M4(path4, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(0, 0, 0));
	M4.isActive = false;
	gameobjects.push_back(M4);
	gameobjects[1].localPosition = glm::vec3(1, 0, 0);
	gameobjects[1].localRotation = glm::vec3(-PI / 2, 0, -PI / 2);

	//Gameobject dao(path5, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	//dao.isActive = false;
	//gameobjects.push_back(dao);

	GameObject dragon(path1, glm::vec3(4.0218, 10.5945, -0.371), glm::vec3(0.0020f, 0.0020f, 0.0020f));
	dragon.isActive = false;
	gameobjects.push_back(dragon);
}