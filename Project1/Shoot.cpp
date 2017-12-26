#include "Shoot.h"

Shoot::Shoot(GameObject * bulletprefab_, vector<GameObject>* gameobjects_, Camera *cam_, float shootInterval_, float flySpeed_):bulletprefab(bulletprefab_), gameobjects(gameobjects_), shootInterval(shootInterval_), flySpeed(flySpeed_),cam(cam_)
{

}

Shoot::~Shoot()
{
}

void Shoot::update(GLFWwindow* window, GameObject * obj)
{
	float currentTime = glfwGetTime();
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		if (currentTime - lastShootTime > shootInterval) {//©иртиД╩В
			lastShootTime = currentTime;
			//cout << "cam->Front is " << cam->Front.x << " " << cam->Front.y << " " << cam->Front.z << " " << endl;
			MonoBehaviour *fly = new Flying(cam->Front, flySpeed);
			GameObject temp = *bulletprefab;
			bulletprefab->scripts.push_back(fly);
			bulletprefab->setPosition(obj->position);
			gameobjects->push_back(*bulletprefab);
			bulletprefab->scripts.clear();
		}
	}
}
