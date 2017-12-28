#include "Gravity.h"



Gravity::Gravity(float rate_):rate(rate_), fallVelocity(glm::vec3())
{
	lastTime = glfwGetTime();
}


Gravity::~Gravity()
{
}

void Gravity::update(GLFWwindow * window, GameObject * obj)
{
	float nowTime = glfwGetTime();
	float deltaTime = nowTime - lastTime;
	lastTime = nowTime;
	if (obj->parent == NULL) {
		fallVelocity = fallVelocity + glm::vec3(0, -gravityRate, 0) * deltaTime * rate;
		//cout << "velocity is " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
		obj->position = obj->position + fallVelocity *deltaTime;
	}
}
