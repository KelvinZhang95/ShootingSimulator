#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "background.h"

#include "shader.h"
#include "Camera.h"
#include "Model.h"
#include "GameObject.h"
#include "Flying.h"
#include "Shoot.h"
#include "EnemyAction.h"
#include "text_render_helper.h"
#include "gui_render_helper.h"
#include "mini_map.h"

#include <stdlib.h> 
#include <time.h> 
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <memory>
using namespace std;
#define PI 3.1415926f
#define miny 4.8
#define maxy 5.7
#define circlexy 12
glm::mat4 TRSum;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);


void creatGameobject();
void adjust();
void switchWeapon();
void processInput(GLFWwindow *window);
void updateLogic(GLFWwindow *window);
void updateScript(GLFWwindow *window);
void updateKinematics();
void updateChildPosRot();
bool positionJudge(glm::vec3 &position);

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
glm::vec3 lightPos(10.0f, 10.0f, 10.0f);
Camera camera(glm::vec3(0.0f, 5.7f, 8.88f));
vector<GameObject> gameobjects;
vector<GameObject> prefabs;
vector<GameObject *> enemies;

bool isPerspective = true;
int nowWeapon = 1;
bool scope = false;

AABBTree aabb_tree_ground;
AABBTree aabb_tree_others;

int score = 0;