#include "Flying.h"


Flying::Flying(glm::vec3 &velocity_, float speed_):velocity(velocity_),speed(speed_)
{
	lastTime = glfwGetTime();
}


Flying::~Flying()
{
}
void Flying::update(GLFWwindow* window, GameObject *obj) {
	float nowTime = glfwGetTime();
	float deltaTime = nowTime - lastTime;
	lastTime = nowTime;
	if (obj->parent == NULL) {
		//cout << "velocity is " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
		obj->position = obj->position + velocity * speed * deltaTime;
	}
}
