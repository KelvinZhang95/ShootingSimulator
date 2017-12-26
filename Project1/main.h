#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h"
//#include "imgui_helper.h"
#include "background.h"

#include "shader.h"
#include "Camera.h"
#include "Model.h"
#include "GameObject.h"
#include "Flying.h"
#include "Shoot.h"


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
void adjust();
void processInput(GLFWwindow *window);
void updateLogic(GLFWwindow *window);
void updateScript(GLFWwindow *window);
void updateKinematics();
void updateChildPosRot();
//void setTexture(unsigned int *texture, string  filepath, bool ispng);

// settings
extern unsigned int SCR_WIDTH = 1200;
extern unsigned int SCR_HEIGHT = 800;

float speed = 2.0f;
bool isSpeed = false;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

bool cursorMode = true;
glm::vec3 lightPos(8.0f, 6.0f, 9.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 11.0f));
vector<GameObject> gameobjects;
vector<GameObject> prefabs;

bool isPerspective = true;
//ImguiHelper imgui_helper;