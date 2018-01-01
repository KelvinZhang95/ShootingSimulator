#include "Flying.h"


Flying::Flying(glm::vec3 &velocity_, float speed_):velocity(velocity_),speed(speed_), survivalTime(10.0f)
{

	lastTime = glfwGetTime();
	bornTime = lastTime;
}


Flying::~Flying()
{
}
void Flying::update(GLFWwindow* window, GameObject *obj) {
	float nowTime = glfwGetTime();
	//if (nowTime - bornTime > survivalTime)
	//	obj->isActive = false;
	float deltaTime = nowTime - lastTime;
	lastTime = nowTime;
	if (obj->parent == NULL) {
		//cout << "velocity is " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
		obj->position = obj->position + velocity * speed * deltaTime;
	}
}
