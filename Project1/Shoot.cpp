#include "Shoot.h"

Shoot::Shoot(GameObject * bulletprefab_, vector<GameObject>* gameobjects_, Camera *cam_, bool isOffset_, float shootInterval_, float flySpeed_):bulletprefab(bulletprefab_), gameobjects(gameobjects_), shootInterval(shootInterval_), flySpeed(flySpeed_),cam(cam_),isOffset(isOffset_)
{

}

Shoot::~Shoot()
{
}

void Shoot::update(GLFWwindow* window, GameObject * obj)
{
	float currentTime = glfwGetTime();
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		if (currentTime - lastShootTime > shootInterval) {//可以射击
			lastShootTime = currentTime;
			//cout << "cam->Front is " << cam->Front.x << " " << cam->Front.y << " " << cam->Front.z << " " << endl;
			MonoBehaviour *fly = new Flying(cam->Front, 10.0);
			GameObject temp = *bulletprefab;
			temp.scripts.push_back(fly);

			MonoBehaviour *gravity = new Gravity(0.0);
			temp.scripts.push_back(gravity);
			if(isOffset){//调整不同枪支的出弹口
				temp.setPosition(obj->position + glm::vec3(cam->Front.x * 0.45, cam->Front.y * 0.45 + cam->Up.y * 0.12, cam->Front.z * 0.45));
			}
			else
			{
				temp.setPosition(obj->position);
			}
			temp.setRotation(cam->rotation);
			temp.rotateInWorld(glm::vec3(0, 180 * PI / 180, 0));
			gameobjects->push_back(temp);
			temp.scripts.clear();
			//bulletprefab->scripts.clear();
		}
	}
}
