#ifndef FLYING_H
#define FLYING_H
#include "MonoBehaviour.h"
#include "GameObject.h"
class GameObject;
class Flying :public MonoBehaviour
{
public:
	float survivalTime;
	glm::vec3 velocity;
	float lastTime;
	float bornTime;
	float speed;
	Flying(glm::vec3 &velocity_ = glm::vec3(), float speed_ = 0);
	~Flying();
	void update(GLFWwindow* window, GameObject *obj);
};
#endif // !FLYING_H

