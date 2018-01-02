#include "enemyAction.h"



EnemyAction::EnemyAction(vector<Particles> *ParticlesList_, int ParticlesIndex_, float rate_) :lastTime(0.0f),randAngle(0.0f),lastChangeTime(0.0f),interval(3.0f),rate(rate_), ParticlesList(ParticlesList_), ParticlesIndex(ParticlesIndex_)
{
	offsetTime = rand() % 10;
}


EnemyAction::~EnemyAction()
{
}

void EnemyAction::update(GLFWwindow * window, GameObject * obj)
{
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	float mindis = 50.0f, maxdis = 1000.f;
	bool flag = true;
	glm::vec3 randRotation = obj->rotation;
	glm::vec3 randPosition = obj->position;
	if (currentTime - lastChangeTime > interval)
	{
		randAngle = (float)(rand() % 60 - 60) / (float)180 * 3.14159;
		randAngle1 = (float)(rand() % 12 - 8) / (float)180 * 3.14159;
		lastChangeTime = currentTime;
	}
	obj->rotateInWorld(glm::vec3(deltaTime / interval * randAngle1, deltaTime / interval * randAngle, 0));
	glm::vec4 front4(0, 0, 1, 0);
	glm::mat4 rot = glm::eulerAngleXYZ(obj->rotation.x, obj->rotation.y, obj->rotation.z);
	front4 = rot * front4;
	glm::vec3 front(front4.x, front4.y, front4.z);
	//cout << "dragon toward" << glm::normalize(front).x << " " << glm::normalize(front).y << " " << glm::normalize(front).z << endl;
	//cout << "dragon position" << obj->position.x << " " << obj->position.y << " " << obj->position.z << endl;
	obj->position.x += glm::normalize(front).x * rate;
	obj->position.y += glm::normalize(front).y * rate;
	obj->position.z += glm::normalize(front).z * rate;
	obj->model.collider.updateOnScalePos(obj->scale,obj->position);
	//cout << "dragon fire " << (int)currentTime << endl;
	bool temp = ((int)(currentTime + offsetTime) % 10) > 4;
	(*ParticlesList)[ParticlesIndex].setIfDisplay(temp);

}

