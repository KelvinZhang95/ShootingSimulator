#include "Shoot.h"

Shoot::Shoot(GameObject * bulletprefab_, vector<GameObject>* gameobjects_, Camera *cam_, vector<GameObject *> *enemies_, bool isOffset_, float shootInterval_, float flySpeed_, float gravityRate_):enemies(enemies_),bulletprefab(bulletprefab_), gameobjects(gameobjects_), shootInterval(shootInterval_), flySpeed(flySpeed_),cam(cam_),isOffset(isOffset_),gravityRate(gravityRate_)
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
			MonoBehaviour *fly = new Flying(cam->Front, flySpeed);
			GameObject temp = *bulletprefab;
			temp.scripts.push_back(fly);

			MonoBehaviour *gravity = new Gravity(gravityRate);
			temp.scripts.push_back(gravity);
			if(isOffset){//调整不同枪支的出弹口
				temp.setPosition(obj->position + glm::vec3(cam->Front.x * 0.45, cam->Front.y * 0.45 + cam->Up.y * 0.12, cam->Front.z * 0.45));
			}
			else
			{
				temp.setPosition(obj->position);
			}

			MonoBehaviour *hitjudge = new HitJudge(enemies);
			temp.scripts.push_back(hitjudge);

			temp.setRotation(cam->rotation);
			temp.rotateInWorld(glm::vec3(0, 180 * PI / 180, 0));
			gameobjects->push_back(temp);
			//cam->setParent(gameobjects->back());
			//(*gameobjects)[1].isActive = false;
			temp.scripts.clear();
			//bulletprefab->scripts.clear();
			cout << "gameobjects size " << gameobjects->size() << endl;
			cout << "gameobjects capcity " << gameobjects->capacity() << endl;
		}
	}
}
